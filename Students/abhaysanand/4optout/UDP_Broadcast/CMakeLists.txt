cmake_minimum_required(VERSION 2.6)
project(UDP_Broadcast)

#include_directories ("${PROJECT_SOURCE_DIR}/json")

add_library(WS2_32 STATIC IMPORTED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(UDP_Broadcast UDP_Broadcast.cpp jsoncpp.cpp stdafx.cpp)

target_link_libraries (UDP_Broadcast "${PROJECT_SOURCE_DIR}/lib/WS2_32.lib")