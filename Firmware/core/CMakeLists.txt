set(NAME core)

file(GLOB SOURCES
	"*.h"
	"*.cpp"
)

add_library(${NAME} INTERFACE)

target_link_libraries(${NAME} INTERFACE
    libs
)

target_sources(libs
	INTERFACE
		"${SOURCES}"
)

target_include_directories(${NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
