cmake_minimum_required(VERSION 2.8.9)
project(2optout)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES_1 cURL_JSON_Conv.cpp)
add_executable(cURL_JSON ${SOURCE_FILES_1})

set(SOURCE_FILES_2 JSON_2_Markdown.cpp)
add_executable(JSON_Markdown ${SOURCE_FILES_2})

