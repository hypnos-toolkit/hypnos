cmake_minimum_required(VERSION 3.6)
project(Hypnos)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    Hypnos.cpp
    Hypnos.h libs/Wire.h libs/Wire.cpp libs/mock_arduino.cpp libs/mock_arduino.h libs/TicksMock.cpp libs/TicksMock.h)

add_library(hypnos ${SOURCE_FILES})

add_subdirectory(test)