cmake_minimum_required(VERSION 2.6)
project(2optout)

include_directories ("${PROJECT_SOURCE_DIR}/json")
add_subdirectory (json)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lcurl -std=c++11")

set(SOURCE_FILES jsonWrap.cpp fetchData.cpp fetchData.h parseData.cpp parseData.h JsonToMarkdown.cpp)
add_executable(2optout ${SOURCE_FILES})

target_link_libraries(2optout  -lcurl json)