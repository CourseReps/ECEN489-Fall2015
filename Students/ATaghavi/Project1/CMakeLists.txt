cmake_minimum_required(VERSION 3.3)
project(project1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(bluetoothToDb bluetoothToDb.cpp)
target_link_libraries(bluetoothToDb -lboost_system -lsqlite3)