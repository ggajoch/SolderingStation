set(NAME simulation)

file(GLOB SOURCES
        "*.h"
        "*.cpp"
)

add_executable(${NAME} ${SOURCES})

target_link_libraries(${NAME}
	libs
	core
	pthread
	ncurses
)

target_format_sources(${NAME} "${SOURCES}")