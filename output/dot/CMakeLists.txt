# Cmake file for otis
cmake_minimum_required(VERSION 2.8.4)

# need c++11
add_definitions(-std=c++11)

# set the library
add_library(dot_output dot_output.cpp)
# and the includes
target_include_directories(dot_output PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} navigator)

target_link_libraries(dot_output navigator)


