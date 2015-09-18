cmake_minimum_required(VERSION 3.3)
project(pp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES pp.cpp sqlite3.c sqlite3.h)
add_executable(untitled ${SOURCE_FILES})