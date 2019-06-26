cmake_minimum_required(VERSION 2.8)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 " )
set( SOURCE_FILES faceFeature_extr.cpp )

project( test )

include( CTest )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( test1 ${SOURCE_FILES} )
target_link_libraries( test1 m ${OpenCV_LIBS} )
add_test(test main)
