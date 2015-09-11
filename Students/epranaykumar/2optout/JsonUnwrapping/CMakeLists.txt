cmake_minimum_required(VERSION 2.6)
project(2optout_markdown)

include_directories ("${PROJECT_SOURCE_DIR}/json")
add_subdirectory (json)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES JsontoMarkdown.cpp)
add_executable(2optout_markdown ${SOURCE_FILES})

target_link_libraries(2optout_markdown  json)