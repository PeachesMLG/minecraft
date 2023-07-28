git cmake_minimum_required(VERSION 3.23)
project(minecraft)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(dependencies/glad)
add_subdirectory(dependencies/glfw)

add_executable(minecraft src/main.cpp)

target_link_libraries(minecraft PUBLIC glfw glad)