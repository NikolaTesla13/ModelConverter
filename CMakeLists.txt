cmake_minimum_required(VERSION 3.26)
project(ModelConverter)

set(CMAKE_CXX_STANDARD 20)

add_executable(ModelConverter src/Source.cpp
        src/ArgParser.cpp
        src/ArgParser.h
        src/Model.h
        src/GLTFModel.cpp
        src/GLTFModel.h
)

add_subdirectory(lib/tinygltf)
include_directories(lib/tinygltf)

target_link_libraries(ModelConverter tinygltf)
