

add_library (PChess PGame.cpp PViewSide.cpp PCheckboard.cpp PFigure.cpp PPoint.cpp PSaver.cpp)
add_executable (ProjectB main.cpp)
message("CMaking project for " ${CMAKE_SYSTEM_NAME})
if (${CMAKE_SYSTEM_NAME} STREQUAL Linux)
    target_link_libraries(PChess bsd)
endif ()

target_link_libraries (ProjectB PChess)
