set(NAME core)

file(GLOB_RECURSE SOURCES
	"*.h"
	"*.cpp"
)

add_library(${NAME} INTERFACE)

target_link_libraries(${NAME} INTERFACE
    libs
    gsl
)

target_sources(libs
	INTERFACE
		"${SOURCES}"
)

target_include_directories(${NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_format_sources(${NAME} "${SOURCES}")