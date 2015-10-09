cmake_minimum_required(VERSION 2.8)
project(ThermistorReading)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories ("${PROJECT_SOURCE_DIR}/sqlite3")
add_subdirectory (sqlite3)

include_directories ("${PROJECT_SOURCE_DIR}/json")
add_subdirectory (json)


add_executable(TeensyClient TeensyClient.cpp)
add_executable(TeensyServer TeensyServer.cpp)

target_link_libraries(TeensyClient sqlite3 json -lpthread -ldl -lpthread)
target_link_libraries(TeensyServer json -lpthread)
