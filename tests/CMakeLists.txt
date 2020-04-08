include_directories (${PROJECTB_SOURCE_DIR}/include)

add_executable (testPPoint testPPoint.cpp)
target_link_libraries (testPPoint PChess)
add_test (NAME PPointTest COMMAND testPPoint)