cmake_minimum_required(VERSION 3.10.0)
project(ReMining VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_C_STANDARD 2Y)
set(CMAKE_CXX_STANDARD 20)
set(OpenGL_GL_PREFERENCE "GLVND")

add_compile_options(-fpermissive)

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3 SDL3-shared)
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_executable(ReMining
    Source/ReMin.cpp)
target_compile_definitions(ReMining PUBLIC SDL_MAIN_USE_CALLBACKS=1)
target_link_libraries(ReMining PUBLIC SDL3::SDL3 SDL3::SDL3-shared ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES} webp)
target_include_directories(ReMining PUBLIC Source Source/Level)