# Cmake file for otis
cmake_minimum_required(VERSION 2.8.4)

# need c++11
add_definitions(-std=c++11)

# fluka test source
set (read_dot_test_source read_dot_test.cpp read_dot_test_driver.cpp)

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

add_test(NAME ReadDotTest.ValidFile COMMAND read_dot_test "--gtest_filter=DotReaderTest.ValidFile
" "--dot_file=${CMAKE_CURRENT_SOURCE_DIR}/dot.dot")
add_test(NAME ReadDotTest.InValidFile COMMAND read_dot_test "--gtest_filter=DotReaderTest.InValidFile
" "--dot_file=${CMAKE_CURRENT_SOURCE_DIR}/dot.dot")
add_test(NAME ReadDotTest.GetNetwork COMMAND read_dot_test "--gtest_filter=DotReaderTest.GetNetwork
" "--dot_file=${CMAKE_CURRENT_SOURCE_DIR}/dot.dot")
add_test(NAME ReadDotTest.Bidirectional COMMAND read_dot_test "--gtest_filter=DotReaderTest.Bidirectional
" "--dot_file=${CMAKE_CURRENT_SOURCE_DIR}/dot.dot")
add_test(NAME ReadDotTest.NotBidirectional COMMAND read_dot_test "--gtest_filter=DotReaderTest.NotBidirectional
" "--dot_file=${CMAKE_CURRENT_SOURCE_DIR}/dot.dot")
