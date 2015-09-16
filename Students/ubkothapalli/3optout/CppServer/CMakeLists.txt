cmake_minimum_required(VERSION 2.8.9)
project(3optout)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES server.cpp )
add_executable(server ${SOURCE_FILES})

#set(SOURCE_FILES client.cpp)
#add_executable(client ${SOURCE_FILES})

target_link_libraries(server -lboost_system -lpthread)
#target_link_libraries(client -lboost_system -lpthread)
