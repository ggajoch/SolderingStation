add_subdirectory(external)

set(NAME libs)

set(SOURCES
	CLI.cpp
	CLI.h
    Average.h
)

add_library(${NAME} INTERFACE)

target_sources(libs
	INTERFACE
		"${CMAKE_CURRENT_LIST_DIR}/CLI.cpp"
)

target_include_directories(${NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_format_sources(${NAME} "${SOURCES}")