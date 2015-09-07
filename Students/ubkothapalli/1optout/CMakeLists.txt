cmake_minimum_required(VERSION 2.8.9)
project(Hamming74)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(Hamming74 ${SOURCE_FILES})