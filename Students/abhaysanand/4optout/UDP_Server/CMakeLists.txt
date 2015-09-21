cmake_minimum_required(VERSION 2.6)
project(UDP_Server)

include_directories ("${PROJECT_SOURCE_DIR}/json")
include_directories ("${PROJECT_SOURCE_DIR}/sqlite3")

add_library(libsqlite3 STATIC IMPORTED)
add_library(WS2_32 STATIC IMPORTED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(UDP_Server UDP_Server.cpp jsoncpp.cpp stdafx.cpp)

target_link_libraries (UDP_Server "${PROJECT_SOURCE_DIR}/lib/libsqlite3.a")
target_link_libraries (UDP_Server "${PROJECT_SOURCE_DIR}/lib/WS2_32.lib")