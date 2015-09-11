cmake_minimum_required(VERSION 3.3)
project(Assignment2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES SpeedData.h)
add_executable(Assignment2 ${SOURCE_FILES} Application1.cpp)
add_executable(Application2 ${SOURCE_FILES} Application2.cpp)