cmake_minimum_required(VERSION 2.6)
project(TrailServerClientUDP)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories ("${PROJECT_SOURCE_DIR}/sqlite3")
add_subdirectory (sqlite3)

include_directories ("${PROJECT_SOURCE_DIR}/json")
add_subdirectory (json)

set(SOURCE_FILES server.cpp)
add_executable(TrailServer ${SOURCE_FILES})
target_link_libraries(TrailServer sqlite3 json -lpthread -ldl -lboost_system -lpthread)

set(SOURCE_FILES client.cpp)
add_executable(TrailClient ${SOURCE_FILES})

target_link_libraries(TrailClient sqlite3 json -lpthread -ldl -lboost_system -lpthread)
