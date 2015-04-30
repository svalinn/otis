# Cmake file for otis
cmake_minimum_required(VERSION 2.8.4)

# need c++11
add_definitions(-std=c++11)

# set the library
add_library(read_tally read_tally.cpp)
# and the includes
target_include_directories (read_tally PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

