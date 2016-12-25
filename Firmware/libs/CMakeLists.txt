add_subdirectory(external)

set(NAME libs)

file(GLOB SOURCES
	"*.h"
	"*.cpp"
)

add_library(${NAME} INTERFACE)

target_sources(libs
	INTERFACE
		"${SOURCES}"
)

target_include_directories(${NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_format_sources(${NAME} "${SOURCES}")