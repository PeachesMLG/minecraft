cmake_minimum_required(VERSION 3.23)
project(minecraft)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(dependencies/glad)
add_subdirectory(dependencies/glfw)

add_executable(minecraft src/main.cpp src/Renderer/Shader.cpp src/Renderer/Shader.h
        src/Renderer/VBO.cpp
        src/Renderer/VBO.h
        src/Renderer/EBO.cpp
        src/Renderer/EBO.h
        src/Renderer/VAO.cpp
        src/Renderer/VAO.h)

target_link_libraries(minecraft PUBLIC glfw glad)