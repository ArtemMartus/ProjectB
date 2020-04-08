include_directories (${PROJECTB_SOURCE_DIR}/include)

add_executable (Test testPPoint.cpp catchMain.cpp testPCheckboard.cpp)
target_link_libraries (Test PChess)
add_test (NAME PPointTest COMMAND Test)