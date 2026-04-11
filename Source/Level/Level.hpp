#pragma once

#include "LevelListener.hpp"
#include "../Physics/AABB.hpp"

#include <SDL3/SDL_log.h>
#include <SDL3/SDL_stdinc.h>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

// #include <libbz3.h>
#include <zlib.h>

#define byte std::byte

struct ReMiLe1 {
    int width, height, depth;
    size_t size;
};

class Level {
    public:
        int width, height, depth;
        std::vector<LevelListener*> levelListeners = {};
    private:
        // std::vector<byte> blocks;
        // std::vector<int> lightDepths;
        byte* blocks;
        int* lightDepths;

    public:
        Level() {}
        Level(int w, int h, int d) {
            this->width = w;
            this->height = h;
            this->depth = d;
            this->blocks = (byte*)std::malloc(w * h * d * sizeof(byte));
            this->lightDepths = (int*)std::malloc(w * h * sizeof(int));
            int x = 0;
            while (x < w) {
                int y = 0;
                while (y < d) {
                    int z = 0;
                    while (z < h) {
                        int i = (y * this->height + z) * this->width + x;
                        this->blocks[i] = (byte)(y <= d * 2 / 3 ? 1 : 0);
                        // this->blocks[i] = (byte)SDL_roundf(SDL_randf());
                        z++;
                    }
                    y++;
                }
                x++;
            }
            this->calcLightDepths(0, 0, w, h);
            this->load();
        }

        // void load() {
        //     FILE* rawFile = std::fopen("World.ReMiLe", "rb");
        //     if (!rawFile) return;

        //     size_t likelySize = bz3_bound(sizeof(&this->blocks) / sizeof(this->blocks[0]));

        //     std::fseek(rawFile, 0, SEEK_END);
        //     std::size_t fileSize = std::ftell(rawFile);
        //     std::fseek(rawFile, sizeof(likelySize), SEEK_SET);

        //     std::size_t compressedSize = fileSize - sizeof(likelySize);

        //     std::vector<uint8_t> compressed(compressedSize);
        //     std::fread(compressed.data(), 1, compressedSize, rawFile);
        //     std::fclose(rawFile);

        //     std::vector<uint8_t> decompressed(likelySize);
        //     size_t decompSize = likelySize;

        //     bz3_decompress(compressed.data(), decompressed.data(), compressedSize, &decompSize);

        //     // allocate blocks properly
        //     std::free(this->blocks); // only if you previously malloc'ed it
        //     this->blocks = (byte*)std::malloc(decompSize);

        //     std::memcpy(this->blocks, decompressed.data(), decompSize);
        // }

        void load() {
            FILE* rawFile = std::fopen("World.ReMiLe", "rb");
            if (!rawFile) return;
            // std::fseek(rawFile, 0, SEEK_END);
            // std::size_t fileSize = std::ftell(rawFile);
            // std::rewind(rawFile);
            ReMiLe1 world;
            // std::fread(&world, fileSize, 1, rawFile);
            std::fread(&world, sizeof(world), 1, rawFile);
            this->width = world.width; this->height = world.height; this->depth = world.depth;
            // this->blocks = world.data;

            this->blocks = (byte*)std::malloc(world.width * world.height * world.depth * sizeof(byte));
            std::fread(this->blocks, 1, world.width * world.height * world.depth * sizeof(byte), rawFile);

            std::fclose(rawFile);
        }

        // void save() {
        //     try {
        //         FILE* rawFile = std::fopen("World.ReMiLe", "wb");
        //         size_t arraySize = sizeof(&this->blocks) / sizeof(this->blocks[0]);
        //         size_t likelySize = bz3_bound(arraySize);
        //         void* buffer = std::malloc(likelySize);
        //         int bz3err = bz3_compress(1024, (uint8_t*)this->blocks, (uint8_t*)buffer, arraySize, &likelySize);
        //         if (bz3err != BZ3_OK) {
        //             SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Saving world failed with BZip3 error code %d", bz3err);
        //             return;
        //         }
        //         std::fwrite(buffer, 1, likelySize, rawFile);
        //         std::free(buffer);
        //         std::fclose(rawFile);
        //     } catch (std::exception e) {
        //         std::printf("%s", e.what());
        //     }
        // }
        void save() {
            FILE* rawFile = std::fopen("World.ReMiLe", "wb");
            ReMiLe1 world;
            world.width = this->width; world.height = this->height; world.depth = this->depth;
            // world.data = this->blocks;
            std::fwrite(&world, sizeof(ReMiLe1), 1, rawFile);
            std::fwrite(this->blocks, 1, this->width * this->height * this->depth * sizeof(byte), rawFile);
            std::fclose(rawFile);
        }

        void calcLightDepths(int x0, int y0, int x1, int y1) {
            int x = x0;
            while (x < x0 + x1) {
                int z = y0;
                while (z < y0 + y1) {
                    int oldDepth = this->lightDepths[x + z * this->width];
                    int y = this->depth - 1;
                    while (y > 0 && !this->isLightBlocker(x, y, z)) y--;
                    this->lightDepths[x + z * this->width] = y;
                    if (oldDepth != y) {
                        int yl0 = oldDepth < y ? oldDepth : y;
                        int yl1 = oldDepth > y ? oldDepth : y;
                        int i = 0;
                        while (i < this->levelListeners.size()) {
                            this->levelListeners[i]->lightColumnChanged(x, z, yl0, yl1);
                            i++;
                        }
                    }
                    z++;
                }
                x++;
            }
        }

        void addListener(LevelListener &levelListener) {
            this->levelListeners.push_back(&levelListener);
        }

        void removeListener(LevelListener &levelListener) {
            std::erase(this->levelListeners, &levelListener);

            // auto it = std::ranges::find_if(levelListeners.begin(), levelListeners.end(),
            //     [levelListener](const std::unique_ptr<LevelListener>& p) {
            //         return p.get() == levelListener;
            //     });
            // if (it != levelListeners.end())
            //     levelListeners.erase(it);
        }

        bool isTile(int x, int y, int z) {
            if (x < 0 || y < 0 || z < 0 || x >= this->width || y >= this->depth || z >= this->height)
                return false;
            return this->blocks[(y * this->height + z) * this->width + x] == (byte)1;
        }

        bool isSolidTile(int x, int y, int z) {
            return this->isTile(x, y, z);
        }

        bool isLightBlocker(int x, int y, int z) {
            return this->isSolidTile(x, y, z);
        }

        std::vector<AABB> getCubes(AABB aabb) {
            std::vector<AABB> AABBs;
            int x0 = (int)aabb.x0;
            int x1 = (int)(aabb.x1 + 1.f);
            int y0 = (int)aabb.y0;
            int y1 = (int)(aabb.y1 + 1.f);
            int z0 = (int)aabb.z0;
            int z1 = (int)(aabb.z1 + 1.f);
            if (x0 < 0) x0 = 0;
            if (y0 < 0) y0 = 0;
            if (z0 < 0) z0 = 0;
            if (x1 > this->width)  x1 = this->width;
            if (y1 > this->depth)  y1 = this->depth;
            if (z1 > this->height) z1 = this->height;
            int x = x0;
            while (x < x1) {
                int y = y0;
                while (y < y1) {
                    int z = z0;
                    while (z < z1) {
                        if (this->isSolidTile(x, y, z))
                            AABBs.push_back(AABB(x,y,z, x+1, y+1, z+1));
                        z++;
                    }
                    y++;
                }
                x++;
            }
            return AABBs;
        }

        AABB* getCubes(AABB aabb, int *count) {
            AABB AABBs[] = {};
            int x0 = (int)aabb.x0;
            int x1 = (int)(aabb.x1 + 1.f);
            int y0 = (int)aabb.y0;
            int y1 = (int)(aabb.y1 + 1.f);
            int z0 = (int)aabb.z0;
            int z1 = (int)(aabb.z1 + 1.f);
            if (x0 < 0) x0 = 0;
            if (y0 < 0) y0 = 0;
            if (z0 < 0) z0 = 0;
            if (x1 > this->width)  x1 = this->width;
            if (y1 > this->depth)  y1 = this->depth;
            if (z1 > this->height) z1 = this->height;
            int i = 0;
            int x = x0;
            while (x < x1) {
                int y = y0;
                while (y < y1) {
                    int z = z0;
                    while (z < z1) {
                        if (this->isSolidTile(x, y, z)) {
                            AABBs[i] = AABB(x,y,z, x+1, y+1, z+1);
                            i++;
                        }
                        z++;
                    }
                    y++;
                }
                x++;
            }
            *count = i;
            return AABBs;
        }

        float getBrightness(int x, int y, int z) {
            float dark = 0.8f, light = 1.f;
            if (x < 0 || y < 0 || z < 0 || x >= this->width || y >= this->depth || z >= this->height)
                return light;
            if (y < this->lightDepths[x + z * this->width])
                return dark;
            return light;
        }

        void setTile(int x, int y, int z, int type) {
            if (x < 0 || y < 0 || z < 0 || x >= this->width || y >= this->depth || z >= this->height)
                return;
            this->blocks[(y * this->height + z) * this->width + x] = (byte)type;
            this->calcLightDepths(x, z, 1, 1);
            int i = 0;
            while (i < this->levelListeners.size()) {
                this->levelListeners[i]->tileChanged(x, y, z);
                i++;
            }
        }
};