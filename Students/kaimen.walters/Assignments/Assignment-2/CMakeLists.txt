cmake_minimum_required(VERSION 3.3)
project(Code2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp SpeedData.cpp SpeedData.h SpeedData.cpp SpeedData.h)
add_executable(Code2 ${SOURCE_FILES} SpeedData.cpp SpeedData.h SpeedData.cpp SpeedData.h)