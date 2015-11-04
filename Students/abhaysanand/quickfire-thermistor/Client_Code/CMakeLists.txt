cmake_minimum_required(VERSION 2.6)
project(Temp_HeatGun)

#include_directories ("${PROJECT_SOURCE_DIR}/json")

add_library(WS2_32 STATIC IMPORTED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(Temp_HeatGun client.cpp jsoncpp.cpp)

target_link_libraries (Temp_HeatGun "${PROJECT_SOURCE_DIR}/lib/WS2_32.lib")