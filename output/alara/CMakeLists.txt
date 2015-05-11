# Cmake file for otis
cmake_minimum_required(VERSION 2.8.4)

# need c++11
add_definitions(-std=c++11)

# set the library
add_library(alara alara.cpp)
# and the includes
target_include_directories (alara PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} navigator)

# 
target_link_libraries(alara read_tally navigator)

# include the tests
add_subdirectory(test)