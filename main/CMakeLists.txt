cmake_minimum_required (VERSION 3.7)

project (project_raytracing VERSION 0.2.0 LANGUAGES CXX)

# We can build raytracing by building raytracing.cpp
add_executable (project_raytracing raytracing.cpp helper.cpp)

# Find Eigen3 and SDL library
find_package (Eigen3 3.3 REQUIRED NO_MODULE)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# raytracing needs to be linked to Eigen (which exports a CMake target called Eigen3::Eigen)
target_link_libraries (project_raytracing Eigen3::Eigen ${SDL2_LIBRARIES})



