set(NAME core)

file(GLOB SOURCES
	"*.h"
	"*.cpp"
)

add_library(${NAME} STATIC ${SOURCES})

target_link_libraries(${NAME}
    libs
)

target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
