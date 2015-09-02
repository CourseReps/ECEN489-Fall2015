cmake_minimum_required(VERSION 3.3)


project(Code1)



set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


set(SOURCE_FILES main.cpp)


add_executable(Code1 ${SOURCE_FILES})