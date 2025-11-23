cmake_minimum_required(VERSION 3.20)
project(GraphInferenceEngine CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER /usr/bin/clang++-20)
set(CMAKE_C_COMPILER /usr/bin/clang-20)
add_compile_options(-stdlib=libc++)
add_link_options(-stdlib=libc++)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()
add_compile_options(
  -Wall
  -Wextra
  -Wpedantic
  -Werror
)

# clang-tidy
find_program(CLANG_TIDY_EXE NAMES "clang-tidy-20" REQUIRED)
set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}" "-header-filter=.*" "-checks=-*,readability-*,modernize-*,performance-*,bugprone-*")

# Includes from conanfile.py
find_package(GTest REQUIRED)
find_package(benchmark REQUIRED)

# Lib target
add_library(GraphEngine INTERFACE)
target_include_directories(GraphEngine INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Source directories
# add_subdirectory(src)
enable_testing()
add_subdirectory(tests)