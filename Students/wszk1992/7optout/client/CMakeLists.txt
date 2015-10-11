cmake_minimum_required(VERSION 2.8)

project(client)

add_executable(client main.cpp)
target_link_libraries (client json)
install(TARGETS client RUNTIME DESTINATION bin)

