include_directories (${PROJECTB_SOURCE_DIR}/include)

add_executable (Test testPPoint.cpp catchMain.cpp testPCheckboard-Pawn.cpp testPCheckboard-King.cpp testPCheckboard-Rook.cpp testPCheckboard-Knight.cpp testPCheckboard-Bishop.cpp testPCheckboard-Queen.cpp testPCheckboard-buildAllFigures.cpp)
target_link_libraries (Test PChess)
add_test (NAME All_Tests COMMAND Test)