cmake_minimum_required (VERSION 2.6)
project (1optout)
add_executable (1optout ./src/main.cpp ./src/Hamming74.cpp)
target_compile_features (1optout PUBLIC cxx_generalized_initializers)
