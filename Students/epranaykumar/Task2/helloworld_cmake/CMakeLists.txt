cmake_minimum_required(VERSION 2.6)
project(helloworld)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES helloworld.cpp)
add_executable(helloworld ${SOURCE_FILES})