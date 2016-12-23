set(NAME unit_tests)

set(SOURCES
	Average.cpp
	test.cpp
)

add_executable(${NAME} ${SOURCES})

target_compile_options(${NAME} PRIVATE "-fexceptions")

target_link_libraries(${NAME}
	libs
	gtest
)

#set (EXEC_OBJ ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${NAME})
