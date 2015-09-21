cmake_minimum_required(VERSION 2.8)
project(imageTransfer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(CppClient CppClient.cpp)
add_executable(CppServer CppServer.cpp)