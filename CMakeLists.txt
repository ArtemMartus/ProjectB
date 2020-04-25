cmake_minimum_required (VERSION 3.10)
project (ProjectB VERSION 1.0.2 LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 17)

set (LIB_NAME PChessArchive)
set (RUN_NAME Project_Chess)
set (INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

add_library (${LIB_NAME})
add_executable (${RUN_NAME})

target_include_directories (${LIB_NAME} PUBLIC ${INCLUDE_DIR})
target_link_libraries (${RUN_NAME} ${LIB_NAME})

include (CTest)

if (BUILD_TESTING)
    add_subdirectory (tests)
endif ()

add_subdirectory (src)
