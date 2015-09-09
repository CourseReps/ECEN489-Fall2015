cmake_minimum_required(VERSION 3.3)
project(correctcode)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp correcterr.cpp correcterr.h)
add_executable(correctcode ${SOURCE_FILES})