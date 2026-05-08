#pragma once

#include <SDL3/SDL_filesystem.h>
#include <cstdio>
#include <cstdlib>
#include <exception>
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
        inline static std::string currentBlockTexture = "";
        inline static void loadTexture(std::string resourceName, int mode, GLuint* ib, long id) {
                                        // TODO: That's a very bad way to get the base name of file!
                                        //       This WILL cause issues in future!
            if (!blockTextures.contains(resourceName.substr(0, resourceName.size() - 5))) {
                bind(id);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode);
                int x, y, n;
                
                // unsigned char *img = stbi_load(resourceName.c_str(), &x, &y, &n, 0);
                            // C++ extension functions my ass ↓
                std::FILE* webpFile = std::fopen(("Assets/Blocks/" + resourceName).c_str(), "rb");
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

                           // [see the TODO comment at the beginning of this function]
                blockTextures[resourceName.substr(0, resourceName.size() - 5)] = id;
                SDL_Log("Added a texture %s with id %lu", resourceName.substr(0, resourceName.size() - 5).c_str(), id);
            } else {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s does not exist!!", resourceName.c_str());
                // throw std::exception((resourceName + " is not existing!").c_str());
                throw std::exception();
            }
        }
        inline static void loadBlockTextures(int mode) {
            // std::filesystem::directory_iterator di = std::filesystem::directory_iterator(std::filesystem::path("Assets/Blocks"));
            // std::filesystem::path test = std::filesystem::current_path() / "Assets/Blocks";
            // long texCount = std::distance(std::filesystem::recursive_directory_iterator{test}, {});
            // // Allocate a fixed‑size array on the stack to hold texture IDs
            // long i = 0;
            int match_count;
            char ** matches = SDL_GlobDirectory("Assets/Blocks", "*.webp", SDL_GLOB_CASEINSENSITIVE, &match_count);
            GLuint ids[match_count];
            glGenTextures(static_cast<GLsizei>(match_count), ids);
            for (int i = 0; i < match_count; i++) {
                loadTexture(matches[i], mode, ids, i);
            }
            SDL_free(matches);
            
            // for (auto& it : std::filesystem::recursive_directory_iterator{test}) {
            //     if (it.is_regular_file()) loadTexture(it.path(), mode, ids, i);
            //     ++i;
            // }
        }
        inline static void bind(int id) {
            if (id != lastId) {
                glBindTexture(GL_TEXTURE_2D, id);
                lastId = id;
            }
        }
        inline static void bindPRO(std::string texName) {
            if (currentBlockTexture != texName) {
                glBindTexture(GL_TEXTURE_2D, blockTextures[texName]);
                lastId = blockTextures[texName];
                currentBlockTexture = texName;
            }
        }

        // C++ has no static constructors...
        inline static void init() {
            lastId = -9999999;
            loadBlockTextures(GL_NEAREST);
        }
};