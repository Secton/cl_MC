#include <exception>
#include <vector>

#include <GL/glew.h>
#include <GL/glext.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_messagebox.h>

#include "HitResult.hpp"
#include "Player.hpp"
#include "Timer.hpp"
#include "Level/Chunk.hpp"
#include "Level/Level.hpp"
#include "Level/LevelRenderer.hpp"

const bool FULLSCREEN_MODE = false;
int width = 1024, height = 768;
std::vector<float> fogColor;
Timer timer = Timer(60.f);
Level level = Level(256, 256, 64);
std::optional<LevelRenderer> levelRenderer;
Player player = Player(&level);
GLint viewportBuffer[16];
GLuint  selectBuffer[2000];
HitResult* hitResult = nullptr;

SDL_Window* window;

void tick() {
    player.tick();
}

void moveCameraToPlayer(float a) {
    glTranslatef(0.f, 0.f, -0.3f);
    glRotatef(player.xRot, 1.f, 0.f, 0.f);
    glRotatef(player.yRot, 0.f, 1.f, 0.f);
    float x = player.xo + (player.x - player.xo) * a;
    float y = player.yo + (player.y - player.yo) * a/* + 20.f*/;
    float z = player.zo + (player.z - player.zo) * a;
    glTranslatef(-x, -y, -z);
}

void setupCamera(float a) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.f, (float)width / (float)height, 0.05f, 1000.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    moveCameraToPlayer(a);
}

void setupPickCamera(float a, int x, int y) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // viewportBuffer.clear();
    glGetIntegerv(GL_VIEWPORT, viewportBuffer);
    gluPickMatrix(x, y, 5.f, 5.f, viewportBuffer);
    gluPerspective(70.f, (float)width / (float)height, 0.05f, 1000.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    moveCameraToPlayer(a);
}

void pick(float a) {
    // selectBuffer.clear();
    glSelectBuffer(2000, selectBuffer);
    glRenderMode(GL_SELECT);
    setupPickCamera(a, width / 2, height / 2);
    glDisable(GL_TEXTURE_2D);
    levelRenderer->pick(&player);
    glEnable(GL_TEXTURE_2D);
    int hits = glRenderMode(GL_RENDER);
    long closest = 0L;
    std::array<int, 10> names;
    int hitNameCount = 0;
    int i = 0;
    int pos = 0;
    while (i < hits) {
        int j;
        int nameCount = selectBuffer[pos++];
        long minZ = selectBuffer[pos++];
        pos++;
        long dist = minZ;
        if (dist < closest || i == 0) {
            closest = dist;
            hitNameCount = nameCount;
            j = 0;
            while (j < nameCount) {
                names[j] = selectBuffer[pos++];
                j++;
            }
        } else {
            j = 0;
            while (j < nameCount) {
                pos++; j++;
            }
        }
        i++;
    }
    if (hitNameCount > 0) {
        HitResult hr = HitResult(names[0], names[1], names[2], names[3], names[4]);
        hitResult = &hr;
    } else
        hitResult = nullptr;
}

void render(float a) {
    glClear(16640);
    setupCamera(a);
    glEnable(GL_CULL_FACE);
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, 2048);
    glFogf(GL_FOG_DENSITY, 0.2f);
    glFogfv(GL_FOG_COLOR, fogColor.data());
    glDisable(GL_FOG);
    levelRenderer->render(player, 0);
    glEnable(GL_FOG);
    levelRenderer->render(player, 1);
    glDisable(GL_TEXTURE_2D);
    if (hitResult != nullptr) levelRenderer->renderHit(*hitResult);
    glDisable(GL_FOG);
    SDL_GL_SwapWindow(window);
}

bool checkError() {
    int err = glGetError();
    if (err != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_RENDER, "OpenGL Error: %i", err);
        return true;
    }
    return false;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    level.save();
    SDL_DestroyWindow(window);
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    try {
        int col = 920330;
        float fr = 0.5f;
        float fg = 0.8f;
        float fb = 1.0f;
        fogColor = {(float)(col >> 16 & 0xFF) / 255.f,
                    (float)(col >> 8  & 0xFF) / 255.f,
                    (float)(col       & 0xFF) / 255.0f,
                    1.0f};
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

        window = SDL_CreateWindow("Game", 1024, 768, SDL_WINDOW_OPENGL);
        SDL_GL_CreateContext(window);
        
        glewExperimental = GL_TRUE; // Fixes issues with some drivers
        GLenum initGLEW = glewInit();
		if (initGLEW != GLEW_OK) {
            SDL_LogError(SDL_LOG_CATEGORY_RENDER, "GLEW Error %i: %s", initGLEW, glewGetErrorString(initGLEW));
            if (initGLEW != 4) return SDL_APP_FAILURE;
            else SDL_Log("That stupid error again, ignoring... Sorry about that!");
        }
        SDL_Log("%s:%i GLErr: %i (if (0) {ignore;})", __FILE_NAME__, __LINE__, glGetError());

        Textures::init();
        levelRenderer.emplace(&level);

        glEnable(GL_TEXTURE_2D);
        glShadeModel(GL_SMOOTH);
        glClearColor(fr, fg, fb, 0.f);
        glClearDepth(1.0);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glMatrixMode(GL_MODELVIEW);
        SDL_CaptureMouse(true);
        SDL_SetWindowRelativeMouseMode(window, true);

        SDL_GL_SetSwapInterval(-1);
    } catch (std::exception exc) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Failed to start ReMin", exc.what(), window);
        return SDL_APP_FAILURE;
    }
    return SDL_APP_CONTINUE;
}

bool debugOutput = false;

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    switch (event->type) {
        case SDL_EVENT_MOUSE_MOTION:
            player.turn(event->motion.xrel, event->motion.yrel);
            // SDL_Log("xRot: %f, yRot: %f", player.xRot, player.yRot);
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            glDisable(GL_TEXTURE_2D);
            pick(timer.a);
            glEnable(GL_TEXTURE_2D);
            if (event->button.button == SDL_BUTTON_LEFT && hitResult != nullptr) {
                level.setTile(hitResult->x, hitResult->y, hitResult->z, 0);
            } else if (event->button.button == SDL_BUTTON_RIGHT && hitResult != nullptr) {
                int x = hitResult->x;
                int y = hitResult->y;
                int z = hitResult->z;
                if (hitResult->f == 0) --y;
                if (hitResult->f == 1) ++y;
                if (hitResult->f == 2) --z;
                if (hitResult->f == 3) ++z;
                if (hitResult->f == 4) --x;
                if (hitResult->f == 5) ++x;
                level.setTile(x, y, z, 1);
            }
            break;
        
        case SDL_EVENT_KEY_DOWN:
            if (event->key.scancode == SDL_SCANCODE_TAB) level.save();
            if (event->key.scancode == SDL_SCANCODE_U && !event->key.repeat) {
                // SDL_Log("World Description BEGIN");
                // SDL_Log("Width: %i, Depth: %i, Height: %i", level.width, level.depth, level.height);
                // for (int x = 0; x < level.width; x++) {
                //     SDL_Log("\\ X - %i", x);
                //     for (int y = 0; y < level.depth; y++) {
                //         std::printf("%i ", y);
                //         for (int z = 0; z < level.height; z++) {
                //             std::printf(level.isTile(x, y, z) ? "█" : "░");
                //         }
                //         std::printf("\n");
                //     }
                // }
                // SDL_Log("World Description END");
                // SDL_Log("PosX: %f Posy:%f PosZ: %f", player.x, player.y, player.z);
                debugOutput = true;
            }
            if (event->key.scancode == SDL_SCANCODE_F11 && !event->key.repeat) {
                if (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN) {
                    SDL_SetWindowFullscreen(window, false);
                    width = 1024; height = 768;
                }
                else {
                    SDL_SetWindowFullscreen(window, true);
                    SDL_Rect* rect;
                    SDL_GetDisplayBounds(SDL_GetDisplayForWindow(window), rect);
                    width = rect->w; height = rect->h;
                }
                glViewport(0,0,width,height);
            }
            // SDL_Log("X0 %f; Y0 %f; Z0 %f;  X1 %f; Y1 %f; Z1 %f;",
            //     player.bb.x0, player.bb.y0, player.bb.z0, player.bb.x1, player.bb.y1, player.bb.z1);
            break;
    }
    return SDL_APP_CONTINUE;
}

Uint64 beginTime;

Uint64 b_timerTime;
Uint64 t_timerTime;

Uint64 b_tickTime;
Uint64 t_tickTime;

Uint64 b_renderTime;
Uint64 t_renderTime;
SDL_AppResult SDL_AppIterate(void *appstate) {
    // if (debugOutput) beginTime = SDL_GetTicks();
    long lastTime = getTime();

    int frames = 0;
    const bool *keyStates = SDL_GetKeyboardState(nullptr);
    try {
        // if (debugOutput) b_timerTime = SDL_GetTicks();
        timer.advanceTime();
        // if (debugOutput) t_timerTime = SDL_GetTicks() - b_timerTime;
        int i = 0;
        // SDL_Log("Timer ticks: %i, pT: %f", timer.ticks, timer.passedTime);
        // if (debugOutput) b_tickTime = SDL_GetTicks();
        while (i < timer.ticks) {
            tick(); i++;
        }
        // if (debugOutput) t_tickTime = SDL_GetTicks() - b_tickTime;
        // if (debugOutput) b_renderTime = SDL_GetTicks();
        render(timer.a);
        // if (debugOutput) t_renderTime = SDL_GetTicks() - b_renderTime;
        // SDL_RenderDebugText()
        frames++;
        while (getTime() >= lastTime + 1000L) {
            SDL_Log("%i fps, %i", frames, Chunk::updates);
            Chunk::updates = 0;
            lastTime += 1000L;
            frames = 0;
        }
        if (keyStates[SDL_SCANCODE_ESCAPE]) return SDL_APP_SUCCESS;
        if (checkError()) return SDL_APP_FAILURE;
    } catch (std::exception exc) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", exc.what());
        return SDL_APP_FAILURE;
    }
    if (debugOutput) {
    //     Uint64 totalTime = SDL_GetTicks() - beginTime;
    //     /*if (totalTime > 15)*/ SDL_Log("Iterate (Total) - %lo ms", totalTime);
    //     SDL_Log("▍ Timer - %lo ms", t_timerTime);
    //     SDL_Log("▍ Tick system - %lo ms", t_tickTime);
    //     SDL_Log("▍ Rendering - %lo ms", t_renderTime);
    //     SDL_Log("𜷀▂▂ Profiling frame");
        debugOutput = false;
    }
    return SDL_APP_CONTINUE;
}