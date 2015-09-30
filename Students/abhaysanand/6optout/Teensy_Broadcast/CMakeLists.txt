cmake_minimum_required(VERSION 2.6)
project(Teensy_Broadcast)

#include_directories ("${PROJECT_SOURCE_DIR}/json")

add_library(WS2_32 STATIC IMPORTED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(Teensy_Broadcast Teensy_Broadcast.cpp jsoncpp.cpp)

target_link_libraries (Teensy_Broadcast "${PROJECT_SOURCE_DIR}/lib/WS2_32.lib")