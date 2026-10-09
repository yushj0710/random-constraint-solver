cmake_minimum_required(VERSION 3.10)

project(RandomConstraintSolver VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

file(GLOB SOURCES
    src/*.cpp
    src/utils/*.cpp
)

add_executable(RandomConstraintSolver ${SOURCES})

# Add the main library
add_library(random_constraint_solver
    src/utils/bv.cpp
    # ...other source files...
)

target_include_directories(random_constraint_solver PUBLIC include)

# Include the tests directory
add_subdirectory(tests)