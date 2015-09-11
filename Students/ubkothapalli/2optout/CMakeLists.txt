cmake_minimum_required(VERSION 2.8.9)
project(2optout_curl2json)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES cURL_JSON_Conv.cpp)
add_executable(2optout_curl2json ${SOURCE_FILES})
