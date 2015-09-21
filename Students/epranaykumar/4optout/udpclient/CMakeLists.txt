cmake_minimum_required(VERSION 2.6)
project(udpclient)

include_directories ("${PROJECT_SOURCE_DIR}/json")
add_subdirectory (json)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES udpClient.cpp)
add_executable(udpclient ${SOURCE_FILES})

target_link_libraries(udpclient json -lboost_system -lpthread)


