cmake_minimum_required(VERSION 3.8)
project(Ist2017)

find_package(OpenCV REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIRS} )


set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)

add_executable(opencv3_demo ${SOURCE_FILES})

add_executable(Ist2017 ${SOURCE_FILES} Segmentation.cpp Drawline.cpp util.h saliency.cpp)

target_link_libraries(Ist2017 ${OpenCV_LIBS})
