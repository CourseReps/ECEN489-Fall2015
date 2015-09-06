cmake_minimum_required(VERSION 3.3)
project(opt1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp encoder.cpp encoder.h decoder.cpp decoder.h)
add_executable(opt1 ${SOURCE_FILES})