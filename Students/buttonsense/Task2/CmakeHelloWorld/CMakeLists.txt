cmake_minimum_required(VERSION 3.3)
project(Task2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES HelloWorld.cpp Helloworld.h main.cpp)
add_executable(Task2 ${SOURCE_FILES})