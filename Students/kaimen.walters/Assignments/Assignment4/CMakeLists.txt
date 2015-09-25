cmake_minimum_required(VERSION 2.6)
project(Code4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(Code4 ${SOURCE_FILES})
