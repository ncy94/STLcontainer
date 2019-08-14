cmake_minimum_required(VERSION 3.14)
project(STLcontainer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)

# the directory for utils
set(UTILS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/utils)

# this is the include directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${UTILS_DIR})

# this is the source files
set(SOURCE_FILES src/library.cpp)

# the library target
add_library(STLcontainer STATIC ${SOURCE_FILES})

# test the library in a local file
add_executable(test_library app/test_library.cpp)
target_link_libraries(test_library STLcontainer)