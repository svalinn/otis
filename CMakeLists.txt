# Cmake file for otis
cmake_minimum_required(VERSION 2.8.4)

# need c++11
#list( APPEND CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11") 
add_definitions(-std=c++11)

# project
project(OTIS)

# needs iGeom 
set(igeom_include_dirs ${CGM_DIR}/include)
set(igeom_lib_dirs ${CGM_DIR}/lib)

# needs cubit
set(cubit_lib_dirs ${CUBIT_DIR}/bin )

# navigator_source
set(navigator_source navigator/Network.cpp)

# read_tally_source
set(read_tally_source read_tally/read_tally.cpp)

# alara source
set(alara_source alara/alara.cpp)

# executable
add_executable( otis main.cpp ${navigator_source} ${read_tally_source} ${alara_source} )

# target include dirs
include_directories(otis navigator read_tally alara ${CMAKE_CURRENT_SOURCE_DIR} ${igeom_include_dirs})

# cgm libs
set(igeom_libs ${igeom_lib_dirs}/libiGeom.so ${igeom_lib_dirs}/libcgm.so)
set(cubit_libs ${cubit_lib_dirs}/libcubiti19.so ${cubit_lib_dirs}/libcubit_geom.so ${cubit_lib_dirs}/libcubit_util.so) 

#libs
target_link_libraries(otis ${igeom_libs} ${cubit_libs})

# install the executable
#install(target otis runtime destination ${INSTALL_BIN_DIR})
#install(program otis destination bin)


