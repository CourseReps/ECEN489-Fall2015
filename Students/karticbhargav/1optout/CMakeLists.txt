#CMakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.
cmake_minimum_required(VERSION 2.8)
project(hammingCode)

set (hammingCode_VERSION_MAJOR 1)
set (hammingCode_VERSION_MINOR 0)

set(SOURCE_FILES main.cpp encoder.cpp encoder.h decoder.cpp decoder.h)

add_executable(hamming ${SOURCE_FILES})