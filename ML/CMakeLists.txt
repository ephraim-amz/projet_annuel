cmake_minimum_required(VERSION 3.19)
project(libtest)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES linear_model.cpp)
add_library(libtest SHARED ${SOURCE_FILES})