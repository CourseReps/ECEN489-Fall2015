cmake_minimum_required(VERSION 2.8)

project(server)

add_executable(server server.c sqlite3.c )
target_link_libraries (server json)
target_link_libraries (server pthread)
target_link_libraries (server dl)
install(TARGETS server RUNTIME DESTINATION bin)
