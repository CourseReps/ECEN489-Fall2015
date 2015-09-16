cmake_minimum_required(VERSION 2.6)
project(cppserver)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES CppServer.cpp)
add_executable(cppserver ${SOURCE_FILES})
target_link_libraries(cppserver -lboost_system -lpthread)