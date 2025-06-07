cmake_minimum_required(VERSION 3.10)

project(ObjectSerialization)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    test/test_serialization.cpp
)

# Add executable
add_executable(ObjectSerialization ${SOURCES})

# Find and link tinyxml2
find_package(tinyxml2 REQUIRED)
target_link_libraries(ObjectSerialization PRIVATE tinyxml2::tinyxml2)