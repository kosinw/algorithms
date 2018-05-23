# Declare the minimum verison of cmake
cmake_minimum_required(VERSION 3.0.0)

# Set the name of the project
project(astar-viz C CXX)

# Set the output dir depending on the configuration type
set(LIBRARY_OUTPUT_PATH ./${CMAKE_CONFIG})

# Keep track of source files
set(SOURCE main.cc)

# Add new executable
add_executable(astar-viz ${SOURCE})

# Link astar library to executable
target_link_libraries(astar-viz astar)
target_link_libraries(astar-viz ${CMAKE_SOURCE_DIR}/lib/x64/SDL2main.lib)
target_link_libraries(astar-viz ${CMAKE_SOURCE_DIR}/lib/x64/SDL2.lib)