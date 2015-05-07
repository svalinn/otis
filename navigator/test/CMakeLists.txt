# Cmake file for otis
cmake_minimum_required(VERSION 2.8.4)

# need c++11
add_definitions(-std=c++11)

# fluka test source
set (navigator_test_source navigator_test.cpp navigator_test_driver.cpp)

# set libraries needed to link to test cases
set(navigator_test_libs
    pthread
    navigator
    gtest
    pthread
)

add_executable (navigator_test
	${navigator_test_source} )

target_link_libraries (navigator_test
       ${navigator_test_libs} )

target_include_directories (navigator_test PUBLIC navigator ${CMAKE_CURRENT_SOURCE_DIR})


# network tests
add_test(NAME NetworkTest.CheckLink COMMAND navigator_test --gtest_filter=NetworkTest.CheckLink)
add_test(NAME NetworkTest.AddLink COMMAND navigator_test --gtest_filter=NetworkTest.AddLink)
add_test(NAME NetworkTest.AddLinkF COMMAND navigator_test --gtest_filter=NetworkTest.AddLinkF)
add_test(NAME NetworkTest.Adjacent COMMAND navigator_test --gtest_filter=NetworkTest.Adjacent)
add_test(NAME NetworkTest.AdjacentOther COMMAND navigator_test --gtest_filter=NetworkTest.AdjacentOther)
add_test(NAME NetworkTest.Adjacent2Link COMMAND navigator_test --gtest_filter=NetworkTest.Adjacent2Link)
add_test(NAME NetworkTest.CheckBiDirectional COMMAND navigator_test --gtest_filter=NetworkTest.CheckBidirectional)
add_test(NAME NetworkTest.CheckNotBiDirectional COMMAND navigator_test --gtest_filter=NetworkTest.CheckNotBidirectional)

#nav tests
add_test(NAME NavigationTest.RoutesNoDFS COMMAND navigator_test --gtest_filter=NavigationTest.RoutesNoDFS)
add_test(NAME NavigationTest.RoutesDFS COMMAND navigator_test --gtest_filter=NavigationTest.RoutesDFS)
add_test(NAME NavigationTest.RoutesDFSComplex COMMAND navigator_test --gtest_filter=NavigationTest.RoutesDFSComplex)
add_test(NAME NavigationTest.RoutesDFSMidTarget COMMAND navigator_test --gtest_filter=NavigationTest.RoutesDFSMidTarget)

