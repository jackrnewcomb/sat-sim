cmake_minimum_required(VERSION 3.20)
project(sat_sim VERSION 0.1.0 LANGUAGES CXX)

option(SIMCORE_BUILD_TESTS "Build simcore unit tests" ON)
option(SIMCORE_BUILD_SHARED "Build simcore as a shared library (for Unity)" ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Good defaults
if (MSVC)
  add_compile_options(/W4 /permissive-)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_subdirectory(simcore)

if (SIMCORE_BUILD_TESTS)
  enable_testing()
  add_subdirectory(simcore/tests)
endif()