cmake_minimum_required(VERSION 3.15)
project(ProjectB)


set(CMAKE_CXX_STANDARD 11)

include_directories(include)

add_subdirectory(src)
enable_testing ()

add_subdirectory(tests)
