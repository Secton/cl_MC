#pragma once

#include "../HitResult.hpp"
#include "../Player.hpp"
#include "Chunk.hpp"
#include "Frustum.hpp"
#include "Level.hpp"
#include "LevelListener.hpp"
#include "Tesselator.hpp"
#include "Tile.hpp"
#include "../Physics/AABB.hpp"
#include "../Timer.hpp"

#include <GL/gl.h>
#include <GL/glext.h>
#include <SDL3/SDL_log.h>

#include <SDL3/SDL_stdinc.h>
#include <cmath>
#include <vector>

// Nullable behavior of the class from https://stackoverflow.com/a/9663710
#define elseWarn else SDL_LogError(SDL_LOG_CATEGORY_ERROR, "LevelRenderer has not been instanced! This function is not static!");

class LevelRenderer: public LevelListener {
    private:
        bool m_null;
        inline static const int CHUNK_SIZE = 16;
        Level* level = nullptr;
        std::vector<Chunk> chunks = {};
        int xChunks, yChunks, zChunks;
        Tesselator t = Tesselator();

    public:
        void setnull() { m_null = true; }
        bool isnull() const { return m_null; }
        LevelRenderer() {
            m_null = true;
        }
        LevelRenderer(Level* level) : m_null(false) {
            this->level = level;
            level->addListener(*this);
            // level->levelListeners.emplace_back(std::make_unique<LevelRenderer>(this));
            this->xChunks = level->width  / 16;
            this->yChunks = level->depth  / 16;
            this->zChunks = level->height / 16;
            this->chunks.resize(this->xChunks * this->yChunks * this->zChunks, Chunk(level, 0,0,0,0,0,0));
            int x = 0;
            while (x < this->xChunks) {
                int y = 0;
                while (y < this->yChunks) {
                    int z = 0;
                    while (z < this->zChunks) {
                        int x0 = x * 16;
                        int y0 = y * 16;
                        int z0 = z * 16;
                        int x1 = (x + 1) * 16;
                        int y1 = (y + 1) * 16;
                        int z1 = (z + 1) * 16;
                        if (x1 > level->width)  x1 = level->width;
                        if (y1 > level->depth)  y1 = level->depth;
                        if (z1 > level->height) z1 = level->height;
                        Chunk ch = Chunk(level, x0, y0, z0, x1, y1, z1);
                        this->chunks[(x + y * this->xChunks) * this->zChunks + z] = ch;
                        z++;
                    }
                    y++;
                }
                x++;
            }
        }
        ~LevelRenderer() override {
            level->removeListener(*this);
        }

        // std::nullptr_t operator=(std::nullptr_t other) {
        //     return nullptr;
        // }

        void render(Player player, int layer) {
            if (!m_null) {
                Chunk::rebuiltThisFrame = 0;
                Frustum frustum = Frustum::getFrustum();
                int i = 0;
                while (i < this->chunks.size()) {
                    if (frustum.cubeInFrustum(this->chunks[i].aabb))
                        this->chunks[i].render(layer);
                    i++;
                }
            } elseWarn
        }

        void pick(Player* player) {
            if (!m_null) {
                float r = 3.f;
                AABB box = player->bb.grow(r, r, r);
                int x0 = (int)box.x0;
                int x1 = (int)(box.x1 + 1.f);
                int y0 = (int)box.y0;
                int y1 = (int)(box.y1 + 1.f);
                int z0 = (int)box.z0;
                int z1 = (int)(box.z1 + 1.f);
                glInitNames();
                int x = x0;
                while (x < x1) {
                    glPushName(x);
                    int y = y0;
                    while (y < y1) {
                        glPushName(y);
                        int z = z0;
                        while (z < z1) {
                            glPushName(z);
                            if (this->level->isSolidTile(x, y, z)) {
                                glPushName(0);
                                int i = 0;
                                while (i < 6) {
                                    glPushName(i);
                                    this->t.init();
                                    Tile::renderFace(&t, x,y,z, i);
                                    this->t.flush();
                                    glPopName();
                                    i++;
                                }
                                glPopName();
                            }
                            glPopName(); z++;
                        }
                        glPopName(); y++;
                    }
                    glPopName(); x++;
                }
            } elseWarn
        }

        void renderHit(HitResult* h) {
            glEnable(GL_BLEND);
            // glDisable(GL_TEXTURE_2D);
            glBlendFunc(GL_SRC_ALPHA, 1);

            glColor4f(1.f, 1.f, 1.f, SDL_sin(getTime() / 100.0) * 0.2f + 0.4f);
            this->t.init();
            Tile::renderFace(&t, h->x, h->y, h->z, h->f);
            this->t.flush();
            // glEnable(GL_TEXTURE_2D);
            glDisable(GL_BLEND);
        }

        void setDirty(int x0, int y0, int z0, int x1, int y1, int z1) {
            if (!m_null) {
                x0 /= 16; x1 /= 16;
                y0 /= 16; y1 /= 16;
                z0 /= 16; z1 /= 16;
                if (x0 < 0) x0 = 0;
                if (y0 < 0) y0 = 0;
                if (z0 < 0) z0 = 0;
                if (x1 >= this->xChunks) x1 = this->xChunks - 1;
                if (y1 >= this->yChunks) y1 = this->yChunks - 1;
                if (z1 >= this->zChunks) z1 = this->zChunks - 1;

                int x = x0;
                while (x <= x1) {
                    int y = y0;
                    while (y <= y1) {
                        int z = z0;
                        while (z <= z1) {
                            this->chunks[(x + y * this->xChunks) * this->zChunks + z].setDirty();
                            z++;
                        }
                        y++;
                    }
                    x++;
                }
            } elseWarn
        }

        void tileChanged(int x, int y, int z) override {
            this->setDirty(x - 1, y - 1, z - 1, x + 1, y + 1, z + 1);
        }
        void lightColumnChanged(int x, int z, int y0, int y1) override {
            this->setDirty(x - 1, y0 - 1, z - 1, x + 1, y1 + 1, z + 1);
        }
        void allChanged() override {
            this->setDirty(0, 0, 0, this->level->width, this->level->depth, this->level->height);
        }
};