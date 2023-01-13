cmake_minimum_required (VERSION 3.7)

project (project_raytracing VERSION 0.2.0 LANGUAGES CXX)

# Include directories
include_directories("${CMAKE_SOURCE_DIR}/main")
include_directories("${CMAKE_SOURCE_DIR}/functions")

# Add subdirectories
add_subdirectory(main)
add_subdirectory(functions)
