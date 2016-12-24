set(NAME arm)

FILE(GLOB_RECURSE CMSIS 
	Drivers/CMSIS/*.h
	Drivers/CMSIS/*.c
	Drivers/CMSIS/*.s
)

FILE(GLOB_RECURSE ST_HAL 
	Drivers/STM32F1xx_HAL_Driver/*.c
	Drivers/STM32F1xx_HAL_Driver/*.h
)

FILE(GLOB SRC
	Src/*.c
	Src/*.cpp
)

message(STATUS "CMSIS: ${CMSIS}")

include_directories(
	"Drivers/CMSIS/Device/ST/STM32F1xx/Include"
	"Drivers/CMSIS/Include"
	"Drivers/STM32F1xx_HAL_Driver/Inc"
	"Inc"
)

set(SOURCES
	${CMSIS}
	${ST_HAL}
	${SRC}
)

add_executable(${NAME} ${SOURCES})

set_target_properties(${NAME} PROPERTIES LINK_FLAGS "-T ${PLATFORM_LINKER_SCRIPT} -specs=nano.specs -Wl,--gc-sections")

target_link_libraries(${NAME}
    core
	libs
)

target_generate_hex(${NAME})
target_memory_report(${NAME})
target_asm_listing(${NAME})
