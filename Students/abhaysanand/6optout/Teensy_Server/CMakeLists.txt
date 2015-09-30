cmake_minimum_required(VERSION 2.6)
project(Teensy_Server)

include_directories ("${PROJECT_SOURCE_DIR}/json")
include_directories ("${PROJECT_SOURCE_DIR}/sqlite3")

add_library(libsqlite3 STATIC IMPORTED)
add_library(WS2_32 STATIC IMPORTED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(Teensy_Server Teensy_Server.cpp jsoncpp.cpp)

target_link_libraries (Teensy_Server "${PROJECT_SOURCE_DIR}/lib/libsqlite3.a")
target_link_libraries (Teensy_Server "${PROJECT_SOURCE_DIR}/lib/WS2_32.lib")