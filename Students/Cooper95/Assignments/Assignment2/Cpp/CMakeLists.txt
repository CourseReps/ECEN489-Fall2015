cmake_minimum_required(VERSION 3.3)
project(Assign2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES SpeedData.h)
add_executable(Assign2 ${SOURCE_FILES} main.cpp)
add_executable(A2App2 ${SOURCE_FILES} App2.cpp)