cmake_minimum_required(VERSION 2.6)
project(udpserver)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories ("${PROJECT_SOURCE_DIR}/sqlite3")

add_subdirectory (sqlite3)
include_directories ("${PROJECT_SOURCE_DIR}/json")
add_subdirectory (json)

set(SOURCE_FILES udpserver.cpp)
add_executable(udpserver ${SOURCE_FILES})


target_link_libraries(udpserver  sqlite3 json -lpthread -ldl -lboost_system -lpthread)