cmake_minimum_required(VERSION 2.8.9)
project(udpclient)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES udpClient.cpp)
add_executable(udpclient ${SOURCE_FILES})

target_link_libraries(udpclient -lboost_system -lpthread)