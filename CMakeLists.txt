# Cmake file for otis
cmake_minimum_required(VERSION 2.8.4)

# need c++11
#list( APPEND CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11") 
add_definitions(-std=c++11)

# project
project(OTIS)

enable_testing()

#gtest
SET ( GTEST_INC_DIR "" )
ADD_SUBDIRECTORY( gtest EXCLUDE_FROM_ALL )
INCLUDE_DIRECTORIES( ${GTEST_INC_DIR} )

add_subdirectory(read_tally)
add_subdirectory(geom)
add_subdirectory(navigator)
add_subdirectory(output)

add_executable( otis main.cpp )

# target include dirs
include_directories(otis PUBLIC read_tally alara_output navigator read_cgm read_dot ${CMAKE_CURRENT_SOURCE_DIR})

# needs iGeom 
set(igeom_include_dirs ${CGM_DIR}/include)
set(igeom_lib_dirs ${CGM_DIR}/lib)

# needs cubit
set(cubit_lib_dirs ${CUBIT_DIR}/bin )

# cgm libs
set(igeom_libs ${igeom_lib_dirs}/libiGeom.so ${igeom_lib_dirs}/libcgm.so)
IF(${CUBIT_VERSION} GREATER 12)
  set(cubit_libs ${cubit_lib_dirs}/libcubiti19.so ${cubit_lib_dirs}/libcubit_geom.so ${cubit_lib_dirs}/libcubit_util.so) 
ELSE(${CUBIT_VERSION} GREATER 12)
  set(cubit_libs ${cubit_lib_dirs}/libcubiti19.so) 
ENDIF(${CUBIT_VERSION} GREATER 12 )

#libs
target_link_libraries(otis read_tally read_dot read_cgm navigator dot_output alara_output ${igeom_libs} ${cubit_libs})

# install the executable
INSTALL( TARGETS otis RUNTIME DESTINATION bin )
#install(program otis destination bin)


