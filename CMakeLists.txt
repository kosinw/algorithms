# Set minimum required CMake version
cmake_minimum_required(VERSION 3.0.0)

# Set project name and language
project(algorithms C CXX)

# Add subdirectories and include directories
add_subdirectory(astar)
add_subdirectory(astar-viz)

target_include_directories(astar-viz PRIVATE ${CMAKE_SOURCE_DIR}/include)