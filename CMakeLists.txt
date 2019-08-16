cmake_minimum_required(VERSION 3.14)
project(STLcontainer)

set(CMAKE_CXX_STANDARD 17)
include_directories(include)

# the directory for utils
set(UTILS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/utils)

# this is the source files
set(SOURCE_FILES src/vector.cpp)

# the library target
add_library(container_library STATIC ${SOURCE_FILES})
target_include_directories(container_library PUBLIC {CMAKE_CURRENT_SOURCE_DIR}/include ${UTILS_DIR})

# test the library in a local file
# QUOTE THESE TWO LINES IF YOU'RE LINKING THIS LIBRARY TO YOUR PROJECT
#add_executable(test_library app/test_library.cpp)
#target_link_libraries(test_library PRIVATE sclib)

add_executable(test_vector app/test_vector.cpp)
target_link_libraries(test_vector PUBLIC container_library)

add_executable(test_list app/test_list.cpp)
target_link_libraries(test_list PUBLIC container_library)