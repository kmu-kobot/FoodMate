get_filename_component(ProjectId ${CMAKE_CURRENT_LIST_DIR} NAME)

string(REPLACE " " "_" ProjectId ${ProjectId})

project(${ProjectId} C CXX)

set (CMAKE_CXX_STANDARD 11)
cmake_minimum_required(VERSION 2.8)
find_package( OpenCV REQUIRED )


# Find source files
file(GLOB SOURCES src/*.cpp)

# Include header files
include_directories(include)


# Create shared library
add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )

