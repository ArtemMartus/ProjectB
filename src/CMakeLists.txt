

add_library (PChess PGame.cpp PViewSide.cpp PCheckboard.cpp PFigure.cpp PPoint.cpp PSaver.cpp PPathSystem.cpp ../include/PPathSystem.h ../include/PFigureFactory.cpp ../include/PFigureFactory.h)
add_executable (ProjectB main.cpp)
target_link_libraries (ProjectB PChess)
