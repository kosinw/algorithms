# Set the name of the project and CXX as the language
project(astar CXX)

# Add static library
add_library(astar STATIC "")

# Target all the sources in this directory
target_sources(astar
	PRIVATE
		"${CMAKE_CURRENT_LIST_DIR}/min_heap.hh"
		"${CMAKE_CURRENT_LIST_DIR}/pathfind.hh"
		"${CMAKE_CURRENT_LIST_DIR}/pathfind.cc"
)

# Include min_heap and pathfind to include path
target_include_directories(astar PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})