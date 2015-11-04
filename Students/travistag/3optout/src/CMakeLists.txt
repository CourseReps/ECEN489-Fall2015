cmake_minimum_required(VERSION 3.3)
project(3optout)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(Server server.cpp)
add_executable(Client client.cpp)
target_link_libraries(Server -lboost_system)
target_link_libraries(Client -lboost_system -lpthread)
