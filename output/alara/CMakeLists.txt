# Cmake file for otis
cmake_minimum_required(VERSION 2.8.4)

# need c++11
add_definitions(-std=c++11)

# set the library
add_library(alara_output alara.cpp)
# and the includes
target_include_directories (alara_output PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# 
target_link_libraries(alara_output read_tally)
