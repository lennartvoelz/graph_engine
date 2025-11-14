cmake_minimum_required(VERSION 3.20)
project(GraphInferenceEngine CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# clang-tidy
find_program(CLANG_TIDY_EXE NAMES "clang-tidy" REQUIRED)
set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}" "-header-filter=.*" "-checks=-*,readability-*,modernize-*,performance-*,bugprone-*")

# Includes from conanfile.py
find_package(GTest REQUIRED)
find_package(benchmark REQUIRED)

# Source directories
add_subdirectory(src)
add_subdirectory(tests)

# benchmarks only in Release builds
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_subdirectory(benchmarks)
endif()

enable_testing()