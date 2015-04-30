# Cmake file for otis
cmake_minimum_required(VERSION 2.8.4)

# need c++11
add_definitions(-std=c++11)

# fluka test source
set (read_dot_test_source ${CMAKE_CURRENT_SOURCE_DIR}/read_dot_test.cpp)

# set libraries needed to link to test cases
set(read_dot_test_libs
    pthread
    read_dot
    navigator
    gtest
    pthread
)

add_executable (read_dot_test
	${read_dot_test_source} )

target_link_libraries (read_dot_test
       ${read_dot_test_libs} )

add_test(NAME ReadDotTest COMMAND read_dot_test)


