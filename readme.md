# C++ STL container library 

## Overview
This library implements STL containers in efficient and understandable codes. The purpose of writing this library is to practise and study. The main knowledge used in this library is: memory management, template programming, object-oriented programming. 

The APIs for each container corresponds those in C++ standard library. The author tries his best to achieve the same robustness and efficiency as the C++ standard library, so that this library can be used in a practical project (although it's usually undesirable to use a library over the standard one). If you're also trying to figure out how standard library works, but often gets bugged by the source code, this library might be of great help to you.

## Build instructions
This is a static library using cmake. To use this library, just link it with your executable.
```cmake
target_link_libraries(${target_name} STLcontainer)

```

If you're only going to test the library, the library provides codes that do the tests. To build the test project, unquote the last two lines in `CMakeLists.txt` as instructed, and try the following lines in the top directore:
```
mkdir build
cd build
cmake ..
make
```

## container lists
- vector
- list
- set
- map
- unordered_set
- unordered_map