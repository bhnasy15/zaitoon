
enable_testing()

file(GLOB TEST_SRC ${TEST_DIR}/*.c)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/test)
file(GLOB TEST_BIN_DIR ${CMAKE_BINARY_DIR}/test)

list(APPEND t_lst ${TEST_SRC})
foreach(T_EXT IN LISTS t_lst)
	get_filename_component(T ${T_EXT} NAME_WLE)
	add_executable(${T} ${T_EXT})
	set_target_properties(${T} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${TEST_BIN_DIR}
		ADDITIONAL_CLEAN_FILES ${T}.ppm)
	add_test(NAME ${T}
		COMMAND ${TEST_BIN_DIR}/${T})
	target_link_libraries(${T} PUBLIC zaitoon)
endforeach()

add_executable(record_check ${TEST_DIR}/record/record_check.c)
set_target_properties(record_check PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${TEST_BIN_DIR})

file(GLOB RECORD_SRC ${TEST_DIR}/record/*.ppm)
list(APPEND r_lst ${RECORD_SRC})
foreach( T_EXT IN LISTS r_lst )
	get_filename_component(T ${T_EXT} NAME)
	add_test(NAME "record_check_:_${T}"
		COMMAND ${TEST_BIN_DIR}/record_check "${T_EXT}" "${CMAKE_BINARY_DIR}/${T}")
endforeach()

add_custom_target(update-record 
	COMMAND cp -f "${CMAKE_BINARY_DIR}/*.ppm" "${TEST_DIR}/record/"
	COMMENT "updated record")
