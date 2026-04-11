#pragma once

#include "Tesselator.hpp"
#include "Level.hpp"

class Tile {
    public:
        static void render(Tesselator* t, Level* level, int layer, int x, int y, int z) {
            float br;
            // Notch's texturesheet position logic
            // float u0 = (float)this->tex / 16.f;
            // float u1 = u0 + 0.0624375f;
            // float v0 = 0.f;
            // float v1 = v0 + 0.0624375f;

            // New standalone texture logic (temp)
            float u0 = 0.f;
            float u1 = 1.f;
            float v0 = 0.f;
            float v1 = 1.f;
            float c1 = 1.f;
            float c2 = 0.8f;
            float c3 = 0.6f;
            float x0 = (float)x + 0.f;
            float x1 = (float)x + 1.f;
            float y0 = (float)y + 0.f;
            float y1 = (float)y + 1.f;
            float z0 = (float)z + 0.f;
            float z1 = (float)z + 1.f;
            if (!level->isSolidTile(x, y - 1, z) && (br = level->getBrightness(x, y - 1, z) * c1) == c1 ^ layer == 1) {
                t->color(br, br, br);
                t->tex(u0, v1);
                t->vertex(x0, y0, z1);
                t->tex(u0, v0);
                t->vertex(x0, y0, z0);
                t->tex(u1, v0);
                t->vertex(x1, y0, z0);
                t->tex(u1, v1);
                t->vertex(x1, y0, z1);
            }
            if (!level->isSolidTile(x, y + 1, z) && (br = level->getBrightness(x, y, z) * c1) == c1 ^ layer == 1) {
                t->color(br, br, br);
                t->tex(u1, v1);
                t->vertex(x1, y1, z1);
                t->tex(u1, v0);
                t->vertex(x1, y1, z0);
                t->tex(u0, v0);
                t->vertex(x0, y1, z0);
                t->tex(u0, v1);
                t->vertex(x0, y1, z1);
            }
            if (!level->isSolidTile(x, y, z - 1) && (br = level->getBrightness(x, y, z - 1) * c2) == c2 ^ layer == 1) {
                t->color(br, br, br);
                t->tex(u1, v0);
                t->vertex(x0, y1, z0);
                t->tex(u0, v0);
                t->vertex(x1, y1, z0);
                t->tex(u0, v1);
                t->vertex(x1, y0, z0);
                t->tex(u1, v1);
                t->vertex(x0, y0, z0);
            }
            if (!level->isSolidTile(x, y, z + 1) && (br = level->getBrightness(x, y, z + 1) * c2) == c2 ^ layer == 1) {
                t->color(br, br, br);
                t->tex(u0, v0);
                t->vertex(x0, y1, z1);
                t->tex(u0, v1);
                t->vertex(x0, y0, z1);
                t->tex(u1, v1);
                t->vertex(x1, y0, z1);
                t->tex(u1, v0);
                t->vertex(x1, y1, z1);
            }
            if (!level->isSolidTile(x - 1, y, z) && (br = level->getBrightness(x - 1, y, z) * c3) == c3 ^ layer == 1) {
                t->color(br, br, br);
                t->tex(u1, v0);
                t->vertex(x0, y1, z1);
                t->tex(u0, v0);
                t->vertex(x0, y1, z0);
                t->tex(u0, v1);
                t->vertex(x0, y0, z0);
                t->tex(u1, v1);
                t->vertex(x0, y0, z1);
            }
            if (!level->isSolidTile(x + 1, y, z) && (br = level->getBrightness(x + 1, y, z) * c3) == c3 ^ layer == 1) {
                t->color(br, br, br);
                t->tex(u0, v1);
                t->vertex(x1, y0, z1);
                t->tex(u1, v1);
                t->vertex(x1, y0, z0);
                t->tex(u1, v0);
                t->vertex(x1, y1, z0);
                t->tex(u0, v0);
                t->vertex(x1, y1, z1);
            }
        }

        static void renderFace(Tesselator* t, int x, int y, int z, int face) {
            float x0 = (float)x + 0.f;
            float x1 = (float)x + 1.f;
            float y0 = (float)y + 0.f;
            float y1 = (float)y + 1.f;
            float z0 = (float)z + 0.f;
            float z1 = (float)z + 1.f;
            if (face == 0) {
                t->vertex(x0, y0, z1);
                t->vertex(x0, y0, z0);
                t->vertex(x1, y0, z0);
                t->vertex(x1, y0, z1);
            }
            if (face == 1) {
                t->vertex(x1, y1, z1);
                t->vertex(x1, y1, z0);
                t->vertex(x0, y1, z0);
                t->vertex(x0, y1, z1);
            }
            if (face == 2) {
                t->vertex(x0, y1, z0);
                t->vertex(x1, y1, z0);
                t->vertex(x1, y0, z0);
                t->vertex(x0, y0, z0);
            }
            if (face == 3) {
                t->vertex(x0, y1, z1);
                t->vertex(x0, y0, z1);
                t->vertex(x1, y0, z1);
                t->vertex(x1, y1, z1);
            }
            if (face == 4) {
                t->vertex(x0, y1, z1);
                t->vertex(x0, y1, z0);
                t->vertex(x0, y0, z0);
                t->vertex(x0, y0, z1);
            }
            if (face == 5) {
                t->vertex(x1, y0, z1);
                t->vertex(x1, y0, z0);
                t->vertex(x1, y1, z0);
                t->vertex(x1, y1, z1);
            }
        }
};