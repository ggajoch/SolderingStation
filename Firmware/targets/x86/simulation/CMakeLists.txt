set(NAME simulation)

file(GLOB SOURCES
        "*.h"
        "*.cpp"
)

add_executable(${NAME} ${SOURCES})

FIND_PACKAGE( Boost 1.40 COMPONENTS program_options REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

target_link_libraries(${NAME}
	libs
	core
	pthread
	ncurses
	boost_system
	${Boost_LIBRARIES}
)

target_format_sources(${NAME} "${SOURCES}")