# Cmake file for otis
cmake_minimum_required(VERSION 2.8.4)

# need c++11
add_definitions(-std=c++11)

# add the output mechanisms
add_subdirectory(dot)
add_subdirectory(alara)