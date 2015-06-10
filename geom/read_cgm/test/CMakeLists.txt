# Cmake file for otis
cmake_minimum_required(VERSION 2.8.4)

# need c++11
add_definitions(-std=c++11)

# fluka test source
set (read_cgm_test_source read_cgm_test.cpp read_cgm_test_driver.cpp)

# set libraries needed to link to test cases
set(read_cgm_test_libs
    pthread
    read_cgm
    ${igeom_libs}
    ${cubit_libs}
    navigator
    gtest
    pthread
)

add_executable (read_cgm_test
	${read_cgm_test_source} )

target_include_directories (read_cgm_test PUBLIC read_cgm navigator ${CMAKE_CURRENT_SOURCE_DIR} ${igeom_include_dirs})

target_link_libraries (read_cgm_test
       ${read_cgm_test_libs} )


add_test(NAME ReadCGMTest.ValidFile COMMAND read_cgm_test --gtest_filter=CGMReaderTest.ValidFile
 --cgm_file=${CMAKE_CURRENT_SOURCE_DIR}/test_geom_water_time.sat)
add_test(NAME ReadCGMTest.InValidFile COMMAND read_cgm_test --gtest_filter=CGMReaderTest.InValidFile
 --cgm_file=${CMAKE_CURRENT_SOURCE_DIR}/test_geom_water_time.sat)
add_test(NAME ReadCGMTest.GetNetwork COMMAND read_cgm_test --gtest_filter=CGMReaderTest.GetNetwork
 --cgm_file=${CMAKE_CURRENT_SOURCE_DIR}/test_geom_water_time.sat)
add_test(NAME ReadCGMTest.Bidirectional COMMAND read_cgm_test --gtest_filter=CGMReaderTest.Bidirectional
 --cgm_file=${CMAKE_CURRENT_SOURCE_DIR}/test_geom_water_time.sat)
add_test(NAME ReadCGMTest.NotBidirectional COMMAND read_cgm_test --gtest_filter=CGMReaderTest.NotBidirectional
 --cgm_file=${CMAKE_CURRENT_SOURCE_DIR}/test_geom_water_time.sat)
add_test(NAME ReadCGMTest.CheckProblemMapsTrue COMMAND read_cgm_test --gtest_filter=CGMReaderTest.CheckProblemMapsTrue
 --cgm_file=${CMAKE_CURRENT_SOURCE_DIR}/test_geom_water_time.sat)
add_test(NAME ReadCGMTest.CheckProblemMapsFalse COMMAND read_cgm_test --gtest_filter=CGMReaderTest.CheckProblemMapsFalse
 --cgm_file=${CMAKE_CURRENT_SOURCE_DIR}/test_geom_water.sat)
add_test(NAME ReadCGMTest.CheckVolumesWater COMMAND read_cgm_test --gtest_filter=CGMReaderTest.CheckProblemMapsFalse
 --cgm_file=${CMAKE_CURRENT_SOURCE_DIR}/model1.sat)
add_test(NAME ReadCGMTest.CheckFlowRateMetadata COMMAND read_cgm_test --gtest_filter=CGMReaderTest.CheckFlowRateMetadata
 --cgm_file=${CMAKE_CURRENT_SOURCE_DIR}/test_geom_water_flow.sat)

