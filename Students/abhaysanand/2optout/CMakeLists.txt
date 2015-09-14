cmake_minimum_required(VERSION 2.6)
project(2optout_json_wrap_unwrap)

#include_directories ("${PROJECT_SOURCE_DIR}/json")
#include_directories ("${PROJECT_SOURCE_DIR}/curl")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_library(libcurl_imp STATIC IMPORTED)

add_executable(2optout_json_wrap_unwrap json_curl_optout2.cpp jsoncpp.cpp stdafx.cpp HTTPDownloader.cpp)

target_link_libraries (2optout_json_wrap_unwrap "${PROJECT_SOURCE_DIR}/lib/libcurl_imp.lib")