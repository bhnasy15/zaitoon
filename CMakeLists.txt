cmake_minimum_required(VERSION 3.15)

project(zaitoon LANGUAGES C VERSION 0.1)

set(CMAKE_C_COMPILER clang)
set(SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(TEST_DIR ${CMAKE_CURRENT_LIST_DIR}/test)

add_compile_options(-gdwarf-4 -ggdb -Wall -Wextra)
#math
add_link_options(-lm)

file(GLOB SRC_FILES ${SRC_DIR}/*/*.c ${SRC_DIR}/*.c)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
file(GLOB LIB_DIR ${CMAKE_BINARY_DIR}/lib)

add_library(${PROJECT_NAME} ${SRC_FILES} )
set_target_properties(${PROJECT_NAME} PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY ${LIB_DIR}
		 ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})


include(${TEST_DIR}/CTest.cmake )
