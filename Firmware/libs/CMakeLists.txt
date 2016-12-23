add_subdirectory(external)

set(NAME libs)

set(SOURCES
	CLI.cpp
	CLI.h
    Average.h
)

add_library(${NAME} STATIC ${SOURCES})

target_link_libraries(${NAME}
    
)

target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
