cmake_minimum_required(VERSION 2.6)
project(3optout_client)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_library(WS2_32 STATIC IMPORTED)

add_executable(3optout_client client.cpp stdafx.cpp)

target_link_libraries (3optout_client "${PROJECT_SOURCE_DIR}/lib/WS2_32.lib")