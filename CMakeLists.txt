cmake_minimum_required(VERSION 3.0)
project(test)
set(CMAKE_BUILD_TYPE Debug)

find_package(OpenCV 4.2.0 REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(main_cmake main.cpp func.cpp)


