cmake_minimum_required(VERSION 2.8.9)
project(client)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES client.cpp)
add_executable(client ${SOURCE_FILES})

target_link_libraries(client -lboost_system -lpthread)
