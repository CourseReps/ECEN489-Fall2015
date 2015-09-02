# cmake_minimum_required(VERSION <specify CMake version here>)
project(1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(1 ${SOURCE_FILES})