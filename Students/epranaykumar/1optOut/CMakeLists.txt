cmake_minimum_required(VERSION 2.6)
project(1optOut)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp hamming_matrix.h hamming_matrix.cpp hamming_functions.cpp hamming_functions.h)
add_executable(1optOut ${SOURCE_FILES})