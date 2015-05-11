# Cmake file for otis
cmake_minimum_required(VERSION 2.8.4)

# need c++11
add_definitions(-std=c++11)

# fluka test source
set (alara_test_source alara_test.cpp alara_test_driver.cpp)

# set libraries needed to link to test cases
set(alara_test_libs
    pthread
    read_dot
    alara
    gtest
    pthread
)

add_executable (alara_test
	${alara_test_source} )

target_link_libraries (alara_test
       ${alara_test_libs} )

add_test(NAME AlaraTest.ValidFile COMMAND alara_test --gtest_filter=AlaraTest.ValidFile --dot_file=${CMAKE_CURRENT_SOURCE_DIR}/dot.dot)
add_test(NAME AlaraTest.InValidFile COMMAND alara_test --gtest_filter=AlaraTest.InValidFile --dot_file=${CMAKE_CURRENT_SOURCE_DIR}/dot.dot)
add_test(NAME AlaraTest.GetNetwork COMMAND alara_test --gtest_filter=AlaraTest.GetNetwork --dot_file=${CMAKE_CURRENT_SOURCE_DIR}/dot.dot)
add_test(NAME AlaraTest.Bidirectional COMMAND alara_test --gtest_filter=AlaraTest.Bidirectional --dot_file=${CMAKE_CURRENT_SOURCE_DIR}/dot.dot)
add_test(NAME AlaraTest.NotBidirectional COMMAND alara_test --gtest_filter=AlaraTest.NotBidirectional --dot_file=${CMAKE_CURRENT_SOURCE_DIR}/dot.dot)
add_test(NAME AlaraTest.CheckProblemMapsTrue COMMAND alara_test --gtest_filter=AlaraTest.CheckProblemMapsTrue --dot_file=${CMAKE_CURRENT_SOURCE_DIR}/dot.dot)
add_test(NAME AlaraTest.CheckProblemMapsFalse COMMAND alara_test --gtest_filter=AlaraTest.CheckProblemMapsFalse --dot_file=${CMAKE_CURRENT_SOURCE_DIR}/dot_bad.dot)
