cmake_minimum_required(VERSION 3.7)
project(assault_cube_hack)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -static-libgcc -static-libstdc++")


set(SOURCE_FILES main.cpp)

set(dir "${CMAKE_CURRENT_SOURCE_DIR}/out")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${dir}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${dir}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${dir}/bin")

add_executable(assault_cube_hack ${SOURCE_FILES})