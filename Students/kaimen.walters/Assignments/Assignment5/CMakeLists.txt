cmake_minimum_required(VERSION 2.6)
project(code5)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(code5 ${SOURCE_FILES})
