# Cmake file for otis
cmake_minimum_required(VERSION 2.8.4)

# need c++11
add_definitions(-std=c++11)

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

# set the library
add_library(read_cgm read_cgm.cpp)
# and the includes
target_include_directories (read_cgm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${igeom_include_dirs})
target_link_libraries(read_cgm navigator)

