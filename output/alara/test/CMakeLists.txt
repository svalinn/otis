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

add_test(NAME AlaraTest.ValidFile COMMAND alara_test --gtest_filter=AlaraTest.ValidFile --mcnp_file=${CMAKE_CURRENT_SOURCE_DIR}/mcnp_output.txt)
add_test(NAME AlaraTest.ValidFileName COMMAND alara_test --gtest_filter=AlaraTest.ValidFileName --mcnp_file=${CMAKE_CURRENT_SOURCE_DIR}/mcnp_output.txt)
add_test(NAME AlaraTest.ValidMCNPFileName COMMAND alara_test --gtest_filter=AlaraTest.ValidMCNPFileName --mcnp_file=${CMAKE_CURRENT_SOURCE_DIR}/mcnp_output.txt)
add_test(NAME AlaraTest.SetFileName COMMAND alara_test --gtest_filter=AlaraTest.SetFileName --mcnp_file=${CMAKE_CURRENT_SOURCE_DIR}/mcnp_output.txt)
add_test(NAME AlaraTest.SetMCNPFileName COMMAND alara_test --gtest_filter=AlaraTest.SetMCNPFileName --mcnp_file=${CMAKE_CURRENT_SOURCE_DIR}/mcnp_output.txt)
add_test(NAME AlaraTest.ValidOutput COMMAND alara_test --gtest_filter=AlaraTest.ValidOutput --mcnp_file=${CMAKE_CURRENT_SOURCE_DIR}/mcnp_output.txt)
add_test(NAME AlaraTest.InValidOutput COMMAND alara_test --gtest_filter=AlaraTest.InValidOutput --mcnp_file=${CMAKE_CURRENT_SOURCE_DIR}/mcnp_output.txt)
add_test(NAME AlaraTest.InValidOutputBadMCNPInput COMMAND alara_test --gtest_filter=AlaraTest.InValidOutput --mcnp_file=${CMAKE_CURRENT_SOURCE_DIR}/mcnp_wrong_output.txt)
add_test(NAME AlaraTest.FileExists COMMAND alara_test --gtest_filter=AlaraTest.FileExists --mcnp_file=${CMAKE_CURRENT_SOURCE_DIR}/mcnp_wrong_output.txt)
add_test(NAME AlaraTest.FileNotExists COMMAND alara_test --gtest_filter=AlaraTest.FileNotExists --mcnp_file=${CMAKE_CURRENT_SOURCE_DIR}/mcnp_wrong_output.txt)
