cmake_minimum_required(VERSION 2.6)
project(cppclient)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES CppCLient.cpp)
add_executable(cppclient ${SOURCE_FILES})
target_link_libraries(cppclient -lboost_system -lpthread)