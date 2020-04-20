cmake_minimum_required(VERSION 3.10)
project(ProjectB)

option(BUILD_TEST "Build the unit tests" ON)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

add_subdirectory(src)

if (BUILD_TEST)
    enable_testing()
    add_subdirectory(tests)
endif ()
