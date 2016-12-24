set(NAME core)

file(GLOB SOURCES
	"*.h"
	"*.cpp"
)
set(CMAKE_C_FLAGS "-std=gnu11 -Os ${CWARN} ${CTUNING} ${CMCU} ${RANDOM_DEFS}" CACHE STRING "compile flags" FORCE)
set(CMAKE_CXX_FLAGS "-std=gnu++1z -Os -fno-exceptions -fno-rtti ${CXXWARN} ${CTUNING} ${CMCU} ${RANDOM_DEFS}" CACHE STRING "compile flags" FORCE)

add_library(${NAME} STATIC ${SOURCES})

target_link_libraries(${NAME}
    libs
)

target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
