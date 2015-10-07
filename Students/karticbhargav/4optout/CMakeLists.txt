cmake_minimum_required(VERSION 2.8)
project(JSONBroadcast)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


include_directories ("${PROJECT_SOURCE_DIR}/sqlite3")
add_subdirectory (sqlite3)

include_directories ("${PROJECT_SOURCE_DIR}/json")
add_subdirectory (json)


add_executable(JSONClient JSONClient.cpp)
add_executable(JSONServer JSONServer.cpp)

target_link_libraries(JSONServer sqlite3 json -lpthread -ldl -lpthread)
target_link_libraries(JSONClient json -lpthread)