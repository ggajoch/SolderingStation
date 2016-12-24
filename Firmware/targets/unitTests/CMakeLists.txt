set(NAME unit_tests)

file(GLOB SOURCES
        "*.h"
        "*.cpp"
)

add_executable(${NAME} ${SOURCES})

target_compile_options(${NAME} PRIVATE "-fexceptions")

target_link_libraries(${NAME}
	libs
	core
	gtest
)

target_format_sources(${NAME} "${SOURCES}")