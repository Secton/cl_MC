cmake_minimum_required(VERSION 4.0.0)
project(cl_MC VERSION 0.1.1 LANGUAGES CXX)
set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 20)
set(OpenGL_GL_PREFERENCE "GLVND")

add_compile_options(-fpermissive)

set(chaosBlocks OFF)
if(chaosBlocks)
    message(STATUS "Chaos blocks")
    add_compile_definitions(chaosBlocks)
endif()

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3 SDL3-shared)
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_executable(cl_MC Source/cl_MC.cpp)
target_compile_definitions(cl_MC PUBLIC SDL_MAIN_USE_CALLBACKS=1)
target_link_libraries(cl_MC PUBLIC SDL3::SDL3 SDL3::SDL3-shared ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES} webp)
target_include_directories(cl_MC PUBLIC Source Source/Level Libraries Libraries/Wasmtime/include)