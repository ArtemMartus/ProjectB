

add_library (PChess PGame.cpp PViewSide.cpp PCheckboard.cpp PFigure.cpp PPoint.cpp PSaver.cpp PPathSystem.cpp PFigureFactory.cpp)
add_executable (ProjectB main.cpp)
target_link_libraries (ProjectB PChess)
