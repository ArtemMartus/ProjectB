cmake_minimum_required(VERSION 3.15)
project(ProjectB)

set(CMAKE_CXX_STANDARD 11)

add_executable(ProjectB main.cpp PGame.cpp PGame.h PViewSide.cpp PViewSide.h PCheckboard.cpp PCheckboard.h PFigure.cpp PFigure.h PPoint.cpp PPoint.h PSaver.cpp PSaver.h)

if (UNIX)
    target_link_libraries(ProjectB bsd)
endif (UNIX)
