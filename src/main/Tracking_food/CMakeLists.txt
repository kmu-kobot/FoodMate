cmake_minimum_required(VERSION 2.8)

project("TrackingFood")

message("project name: ${CMAKE_PROJECT_NAME}")
message("install dir: ${CMAKE_SOURCE_DIR}")

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_BUILD_TYPE Release)

find_package(OpenCV REQUIRED)
find_package(Threads)


add_compile_options(-Wall -std=c++11 -lpthread -lJetsonGPIO)
include_directories(${CMAKE_SOURCE_DIR}/include)

set(SOURCE_FILES Board.cpp Sound.cpp Matcher.cpp Tracker.cpp SocketClient.cpp ThDetectRecognizer.cpp ThTracker.cpp main.cpp)

message("set vars: ${CMAKE_BUILD_TYPE}, ${SOURCE_FIELS}")


add_executable(ResultFile ${SOURCE_FILES})
target_link_libraries(ResultFile ${OpenCV_LIBS}
				${CMAKE_THREAD_LIBS_INIT})
