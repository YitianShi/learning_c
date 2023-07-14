cmake_minimum_required(VERSION 3.0)
add_definitions(-DEBUG=1)
project(test)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(INCLUDE_DIRECTORIES "include")
set(SOURCES 
src/main.cpp
src/func.cpp
src/inno.cpp
)

find_package(CUDA REQUIRED)
find_package(OpenCV)

include_directories(${OpenCV_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(/usr/local/cuda/include) # include directories cuda

add_executable(main_cmake ${SOURCES})
target_link_libraries(main_cmake ${OpenCV_LIBS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")





