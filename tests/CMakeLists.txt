include_directories (${PROJECTB_SOURCE_DIR}/include)

add_executable (Test test.cpp test.cpp)
target_link_libraries (Test PChess)
add_test (NAME MyTest COMMAND Test)