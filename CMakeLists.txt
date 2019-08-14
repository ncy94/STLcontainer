cmake_minimum_required(VERSION 3.14)
project(STLcontainer)

set(CMAKE_CXX_STANDARD 17)

add_library(STLcontainer src/library.cpp include/library.h)