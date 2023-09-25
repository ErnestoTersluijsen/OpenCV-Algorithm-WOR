cmake_minimum_required(VERSION 3.10)
project(OpenCV_Algorithm_WOR)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -Wconversion)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(OpenCV_Algorithm_WOR
        src/main.cpp
        src/ImageProcessing.cpp
        src/ImageProcessing.hpp
        src/ShapeDetection.cpp
        src/ShapeDetection.hpp
)

target_link_libraries(OpenCV_Algorithm_WOR ${OpenCV_LIBS})