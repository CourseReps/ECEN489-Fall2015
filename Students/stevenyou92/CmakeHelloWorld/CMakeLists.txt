cmake_minimum_required(VERSION 3.3)
project(task2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp HelloWorld.cpp HelloWorld.h)
add_executable(task2 ${SOURCE_FILES})