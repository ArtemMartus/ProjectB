cmake_minimum_required (VERSION 3.10)
project (ProjectB VERSION 1.0.2 LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 17)

set (LIB_NAME ChessArchive)
set (RUN_NAME Project_Chess)
set (INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

add_library (${LIB_NAME})
add_executable (${RUN_NAME})

target_include_directories (${LIB_NAME} PUBLIC ${INCLUDE_DIR})
target_link_libraries (${RUN_NAME} ${LIB_NAME})

include (CTest)

if (BUILD_TESTING)
    set (TARGET unit_tests)
    add_executable (${TARGET} ${CMAKE_CURRENT_LIST_DIR}/tests/catchMain.cpp)
    target_link_libraries(${TARGET} PUBLIC ${LIB_NAME})

    add_subdirectory (tests)
endif ()

add_subdirectory (src)
