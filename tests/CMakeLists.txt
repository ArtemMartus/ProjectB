include_directories(${PROJECTB_SOURCE_DIR}/include)

add_library(Catch catchMain.cpp)

add_executable(Test-PPoint testPPoint.cpp)
target_link_libraries(Test-PPoint PChess Catch)
add_test(NAME PPointTest COMMAND Test-PPoint)

add_executable(Test-Pawn-Path test-Pawn-Path.cpp)
target_link_libraries(Test-Pawn-Path PChess Catch)
add_test(NAME PawnPath COMMAND Test-Pawn-Path)

add_executable(Test-King-Path test-King-Path.cpp)
target_link_libraries(Test-King-Path PChess Catch)
add_test(NAME KingPath COMMAND Test-King-Path)

add_executable(Test-Rook-Path test-Rook-Path.cpp)
target_link_libraries(Test-Rook-Path PChess Catch)
add_test(NAME RookPath COMMAND Test-Rook-Path)

add_executable(Test-Knight-Path test-Knight-Path.cpp)
target_link_libraries(Test-Knight-Path PChess Catch)
add_test(NAME KnightPath COMMAND Test-Knight-Path)

add_executable(Test-Bishop-Path test-Bishop-Path.cpp)
target_link_libraries(Test-Bishop-Path PChess Catch)
add_test(NAME BishopPath COMMAND Test-Bishop-Path)

add_executable(Test-Queen-Path test-Queen-Path.cpp)
target_link_libraries(Test-Queen-Path PChess Catch)
add_test(NAME QueenPath COMMAND Test-Queen-Path)

add_executable(Test-PCheckboard testPCheckboard.cpp)
target_link_libraries(Test-PCheckboard PChess Catch)
add_test(NAME PChessboard COMMAND Test-PCheckboard)

add_executable(Test-PFigureFactory testPFigureFactory.cpp)
target_link_libraries(Test-PFigureFactory PChess Catch)
add_test(NAME PFigureFactory COMMAND Test-PFigureFactory)

