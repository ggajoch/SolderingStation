set(NAME unit_tests)

file(GLOB SOURCES
        "*.h"
        "*.cpp"
)

add_executable(${NAME} ${SOURCES})

target_link_libraries(${NAME}
	libs
	core
	gtest
	gtest_main
)

target_format_sources(${NAME} "${SOURCES}")