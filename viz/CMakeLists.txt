# Add the visualization program
project (viz)

# Add the visualization executable
add_executable(viz WIN32 "")

# Add appropriate sources to project
target_sources(viz
	PRIVATE
		"${CMAKE_CURRENT_LIST_DIR}/main.cpp"
)

# Include windowing directory
include (windowing/CMakeLists.txt)

# Include system directory
include (system/CMakeLists.txt)

# Link astar library to visualization library
target_link_libraries(viz PUBLIC astar)

# Add header files to this project
target_include_directories(viz PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Set entrypoint to int main(int, char**)
set_target_properties(viz PROPERTIES LINK_FLAGS "/subsystem:CONSOLE /entry:mainCRTStartup")

# Add unicode instead of ANSI to Windows application
add_definitions(-DUNICODE=1)