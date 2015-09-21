cmake_minimum_required(VERSION 2.8.9)
project(server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories ("${PROJECT_SOURCE_DIR}/sqlite3")
add_subdirectory (sqlite3)

set(SOURCE_FILES server.cpp)
add_executable(server ${SOURCE_FILES})


target_link_libraries(server  sqlite3 -lpthread -ldl -lboost_system -lpthread)
