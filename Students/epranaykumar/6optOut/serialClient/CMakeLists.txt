cmake_minimum_required(VERSION 2.6)
project(serialClient)

include_directories ("${PROJECT_SOURCE_DIR}/json")
add_subdirectory (json)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES serialClient.cpp)
add_executable(serialClient ${SOURCE_FILES})

target_link_libraries(serialClient json -lboost_system -lpthread)


