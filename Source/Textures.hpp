#pragma once

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <map>
#include <string>

#include <SDL3/SDL_log.h>
#include <GL/gl.h>
#include <GL/glext.h>
// #define STB_IMAGE_IMPLEMENTATION
// #define STBI_ONLY_PNG
// #include <stb/stb_image.h>
#include <webp/decode.h>


class Textures {
    private:
        inline static int lastId;


    public:
        /** @brief Dictionary of block textures.
            Use Example: `glBindTexture(3553, blockTextures["Grass"]);`
        */
        inline static std::map<std::string, int> blockTextures;
        inline static void loadTexture(std::filesystem::path resourceName, int mode, GLuint* ib, long id) {
            if (std::filesystem::exists(resourceName) && !blockTextures.contains(resourceName.stem())) {
                bind(id);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode);
                int x, y, n;
                
                // unsigned char *img = stbi_load(resourceName.c_str(), &x, &y, &n, 0);
                            // C++ extension functions my ass ↓
                std::FILE* webpFile = std::fopen(resourceName.c_str(), "rb");
                std::fseek(webpFile, 0, SEEK_END);
                long webpSize = std::ftell(webpFile);
                std::fseek(webpFile, 0, SEEK_SET);
                void* webpBuffer = std::malloc(webpSize);
                std::fread(webpBuffer, 1, webpSize, webpFile);
                std::fclose(webpFile);
                int webpWidth, webpHeight;
                void* webpTex = WebPDecodeRGBA((uint8_t*)webpBuffer, webpSize, &webpWidth, &webpHeight);
                std::free(webpBuffer);

                // Source: https://learnopengl.com/Getting-started/Textures
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, webpWidth, webpHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, webpTex);
                WebPFree(webpTex);
                // Mipmaps are intentionally not created.

                blockTextures[resourceName.stem()] = id;
            } else {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s does not exist!!", resourceName.c_str());
                throw "%s is not existing!", resourceName.c_str();
            }
        }
        inline static void loadBlockTextures(int mode) {
            // std::filesystem::directory_iterator di = std::filesystem::directory_iterator(std::filesystem::path("Assets/Blocks"));
            std::filesystem::path test = std::filesystem::path(std::filesystem::current_path().string() + "/Assets/Blocks/");
            long texCount = std::distance(std::filesystem::recursive_directory_iterator {test}, {});
            GLuint* ib;
            glGenTextures(texCount, ib);
            long i = 0;
            
            for (auto& it : std::filesystem::recursive_directory_iterator {test}) {
                if (it.is_regular_file()) loadTexture(it.path(), mode, ib, i);
                i++;
            }
        }
        inline static void bind(int id) {
            if (id != lastId) {
                glBindTexture(GL_TEXTURE_2D, id);
                lastId = id;
            }
        }

        // C++ has no static constructors...
        inline static void init() {
            lastId = -9999999;
            loadBlockTextures(GL_NEAREST);
        }
};