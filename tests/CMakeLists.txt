include_directories (${PROJECTB_SOURCE_DIR}/include)

add_executable (Test testPPoint.cpp catchMain.cpp test-Pawn-Path.cpp test-King-Path.cpp test-Rook-Path.cpp test-Knight-Path.cpp test-Bishop-Path.cpp test-Queen-Path.cpp testPCheckboard-buildAllFigures.cpp testPFigureFactory.cpp)
target_link_libraries (Test PChess)
add_test (NAME All_Tests COMMAND Test)