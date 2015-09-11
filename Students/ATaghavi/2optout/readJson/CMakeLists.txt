cmake_minimum_required(VERSION 3.3)
project(2optout2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES readJson.cpp)
add_executable(readJson ${SOURCE_FILES})
