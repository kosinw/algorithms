# Set the minimum version of cmake required
cmake_minimum_required(VERSION 3.0.0)

# Set the name of the project and CXX as the language
project(astar CXX)

# Set the output dir depending on the configuration type
set(LIBRARY_OUTPUT_DIRECTORY ./${CMAKE_BUILD_CONFIG})

# Add source files to source variable
set(SOURCE src/pathfind.cc)

# Add new static library
add_library(astar STATIC ${SOURCE})