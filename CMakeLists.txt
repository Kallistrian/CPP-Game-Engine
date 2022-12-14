cmake_minimum_required(VERSION 3.24)
project(CPPGameProject)
add_executable(CPPGame source.cpp gl.c "include/shader.h" "shader.cpp" "include/gameobject.h"  "include/model.h" "model.cpp")

add_library(stb INTERFACE)
add_library(glad INTERFACE)
find_path(STB_INCLUDE_DIRS "stb_c_lexer.h")
target_include_directories(stb INTERFACE ${STB_INCLUDE_DIRS})
target_include_directories(glad INTERFACE include)	
find_package(glfw3 3.3 REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

target_link_libraries(CPPGame PRIVATE glad)
target_link_libraries(CPPGame PRIVATE stb)
target_link_libraries(CPPGame PRIVATE glfw)
target_link_libraries(CPPGame PRIVATE glm::glm)
target_link_libraries(CPPGame PRIVATE assimp::assimp)