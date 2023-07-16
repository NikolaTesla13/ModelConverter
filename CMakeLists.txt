cmake_minimum_required(VERSION 3.26)
project(ModelConverter)

set(CMAKE_CXX_STANDARD 17)

add_executable(ModelConverter src/main.cpp)

add_subdirectory(lib/tinygltf)
include_directories(lib/tinygltf)

target_link_libraries(ModelConverter tinygltf)
