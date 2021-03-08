cmake_minimum_required(VERSION 2.8)

project(ConvertRTK)

SET(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS "-std=c++11")
#set(INC_DIR /home/ztj/software/opencv-3.2.0/include)
#set(OpenCV_INCLUDE_DIR /usr/local/opencv320/include)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -O0")


#find_package(OpenCV 3.0 QUIET)
#include_directories( ${OpenCV_INCLUDE_DIR} )


add_executable(run_main main.cpp
        converter.cpp)
#target_link_libraries(test ${OpenCV_LIBS} )
