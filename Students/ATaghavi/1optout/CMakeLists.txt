cmake_minimum_required(VERSION 3.3)
project(HammingCodes)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp HammingUtil.cpp HammingUtil.h)
add_executable(HammingCodes ${SOURCE_FILES})