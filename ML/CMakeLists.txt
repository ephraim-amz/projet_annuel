cmake_minimum_required(VERSION 3.19)
project(ml_lib)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES ml.cpp)

add_library(ml_lib SHARED ${SOURCE_FILES})