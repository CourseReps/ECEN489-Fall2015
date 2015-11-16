cmake_minimum_required(VERSION 3.1)
project(project1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(server server.cpp)

target_link_libraries(server -lboost_system -lsqlite3)
