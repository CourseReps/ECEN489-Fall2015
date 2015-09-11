cmake_minimum_required(VERSION 3.3)
project(curl)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES getCurlData.cpp)
add_executable(curl ${SOURCE_FILES})
