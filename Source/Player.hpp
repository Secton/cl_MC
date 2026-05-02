#pragma once

#include "Level/Level.hpp"
#include "Physics/AABB.hpp"

#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_stdinc.h>
#include <vector>

class Player {
    private:
        Level* level = nullptr;
    public:
        float xo;
        float yo;
        float zo;
        float x;
        float y;
        float z;
        float xd;
        float yd;
        float zd;
        float yRot;
        float xRot;
        AABB bb = AABB(0,0,0,0,0,0);
        bool onGround = false;

        Player(Level* level) {
            this->level = level;
            this->resetPos();
        }

    private:
        void resetPos() {
            float x = SDL_randf() * (float)this->level->width;
            float y = this->level->depth + 10;
            float z = SDL_randf() * (float)this->level->height;
            this->setPos(x,y,z);
        }

        void setPos(float x, float y, float z) {
            this->x = x; this->y = y; this->z = z;
            float w = 0.3f, h = 0.9f;
            this->bb = AABB(x - w, y - h, z - w, x + w, y + h, z + w);
        }

    public:
        void turn(float xo, float yo) {
            this->yRot = (float)((double)this->yRot + (double)xo * 0.15);
            this->xRot = (float)((double)this->xRot + (double)yo * 0.15);
            if (this->xRot < -90.f) this->xRot = -90.f;
            if (this->xRot >  90.f) this->xRot =  90.f;
            if (this->yRot < -360.f) this->yRot += 360.f;
            if (this->yRot > 360.f) this->yRot -= 360.f;
        }

        void tick() {
            this->xo = this->x;
            this->yo = this->y;
            this->zo = this->z;
            float xa = 0.f, ya = 0.f;
            const bool *keyStates = SDL_GetKeyboardState(nullptr);
            
            // Now now, decompiled code seems to use numbers instead of enums,
            // which makes sense, but it leaves me no choice but to guess the keys.
            // I could've looked up the LWJGL's docs for an old version, but
            // i've decided not to.
            if (keyStates[SDL_SCANCODE_R]) this->resetPos();
            
            if (keyStates[SDL_SCANCODE_W] || keyStates[SDL_SCANCODE_UP   ]) ya -= 1.f;
            if (keyStates[SDL_SCANCODE_S] || keyStates[SDL_SCANCODE_DOWN ]) ya += 1.f;
            if (keyStates[SDL_SCANCODE_A] || keyStates[SDL_SCANCODE_LEFT ]) xa -= 1.f;
            if (keyStates[SDL_SCANCODE_D] || keyStates[SDL_SCANCODE_RIGHT]) xa += 1.f;
            // Well the key numbers seemed a bit weird, and after
            // counting down to D key, it turns out that those are relative to my
            // keyboard layout!
            if ((keyStates[SDL_SCANCODE_SPACE]) && this->onGround) // TODO: figure out what key 219 might be!
                this->yd = 0.12f;
            this->moveRelative(xa, ya, this->onGround ? 0.02f : 0.005f);
            this->yd = (float)((double)this->yd - 0.005);
            this->move(this->xd, this->yd, this->zd);
            this->xd *= 0.91f;
            this->yd *= 0.98f;
            this->zd *= 0.91f;
            if (this->onGround) {
                this->xd *= 0.8f;
                this->zd *= 0.8f;
            }
        }

        void move(float xa, float ya, float za) {
            float xaOrg = xa, yaOrg = ya, zaOrg = za;
            std::vector<AABB> AABBs = this->level->getCubes(this->bb.expand(xa, ya, za));
            int i = 0;
            while (i < AABBs.size()) {
                ya = AABBs[i].clipYCollide(this->bb, ya);
                i++;
            }
            this->bb.move(0.f, ya, 0.f);
            i = 0;
            while (i < AABBs.size()) {
                xa = AABBs[i].clipXCollide(this->bb, xa);
                i++;
            }
            this->bb.move(xa, 0.f, 0.f);
            i = 0;
            while (i < AABBs.size()) {
                za = AABBs[i].clipZCollide(this->bb, za);
                i++;
            }
            this->bb.move(0.f, 0.f, za);
            this->onGround = yaOrg != ya && yaOrg < 0.f;
            if (xaOrg != xa) this->xd = 0.f;
            if (yaOrg != ya) this->yd = 0.f;
            if (zaOrg != za) this->zd = 0.f;
            this->x = (this->bb.x0 + this->bb.x1) / 2.f;
            this->y =  this->bb.y0 + 1.62f;
            this->z = (this->bb.z0 + this->bb.z1) / 2.f;
        }

        void moveRelative(float xa, float za, float speed) {
            float dist = xa * xa + za * za;
            if (dist < 0.01f) return;
            dist = speed / (float)SDL_sqrt(dist);
            float sin = (float)SDL_sin((double)this->yRot * SDL_PI_D / 180.0);
            float cos = (float)SDL_cos((double)this->yRot * SDL_PI_D / 180.0);
            this->xd += (xa *= dist) * cos - (za *= dist) * sin;
            this->zd += za * cos + xa * sin;
        }
};