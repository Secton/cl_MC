#pragma once

#include "../Physics/AABB.hpp"
#include "Level.hpp"
#include "../Textures.hpp"
#include "Tesselator.hpp"
#include "Tile.hpp"

#include <GL/gl.h>

class Chunk {
    public:
        AABB aabb = AABB(0,0,0,0,0,0);
        /*const*/ Level* level = nullptr;
        /*const*/ int x0;
        /*const*/ int y0;
        /*const*/ int z0;
        /*const*/ int x1;
        /*const*/ int y1;
        /*const*/ int z1; // FIXME: you cannot set value of unset const... WHY OH WHY
    private:
        bool dirty = true;
        int lists = -1;
    public:
        inline static Tesselator t;
        inline static int rebuiltThisFrame = 0;
        inline static int updates = 0;

        Chunk(Level *level, int x0, int y0, int z0, int x1, int y1, int z1) {
            this->level = level;
            this->x0 = x0;
            this->y0 = y0;
            this->z0 = z0;
            this->x1 = x1;
            this->y1 = y1;
            this->z1 = z1;
            this->aabb = AABB(x0, y0, z0, x1, y1, z1);
            this->lists = glGenLists(2);
            // t = Tesselator();
            // rebuiltThisFrame = 0;
            // updates = 0;
        }
    
        private:
            void rebuild(int layer) {
                if (rebuiltThisFrame == 2) return;
                this->dirty = false;
                updates++; rebuiltThisFrame++;
                glNewList(this->lists + layer, GL_COMPILE);
                glEnable(GL_TEXTURE_2D);
                t.init();
                int tiles = 0;
                int x = this->x0;
                while (x < this->x1) {
                    int y = this->y0;
                    while (y < this->y1) {
                        int z = this->z0;
                        while (z < this->z1) {
                            if (this->level->isTile(x, y, z)) {
                                bool tex = y != this->level->depth * 2 / 3;
                                tiles++;
                                if (!tex) {
                                    if (Textures::currentBlockTexture != "Grass" && t.vertices > 0) t.flush();
                                    Textures::bindPRO("Grass");
                                    // t.init();
                                    Tile::render(&t, this->level, layer, x,y,z);
                                } else {
                                    if (Textures::currentBlockTexture != "Stone" && t.vertices > 0) t.flush();
                                    Textures::bindPRO("Stone");
                                    // t.init();
                                    Tile::render(&t, this->level, layer, x,y,z);
                                }
                            }
                            z++;
                        }
                        y++;
                    }
                    x++;
                }
                t.flush();
                Textures::currentBlockTexture = "";
                glDisable(GL_TEXTURE_2D);
                glEndList();
            }

        public:
            void render(int layer) {
                if (this->dirty) {
                    this->rebuild(0);
                    this->rebuild(1);
                }
                glCallList(this->lists + layer);
            }

            void setDirty() {this->dirty = true;}
};