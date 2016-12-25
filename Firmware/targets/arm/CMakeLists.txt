set(NAME arm)

add_subdirectory(Drivers)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-strict-prototypes")

include(Drivers/allowed_warnings.cmake)

set(SOURCES_GENERATED
	Src/adc.c
	Src/gpio.c
	Src/main.c
	Src/stm32f1xx_hal_msp.c
	Src/stm32f1xx_it.c
	Src/tim.c
	Src/usart.c
)

set(SOURCES_PROJECT
	Src/coreSupport.cpp
	Src/HAL.cpp
	Src/hd44780.c
	Src/MY_main.cpp
	Src/newlib.c
)

include_directories(
	"Inc"
	"Drivers/CMSIS/Device/ST/STM32F1xx/Include"
	"Drivers/CMSIS/Include"
	"Drivers/STM32F1xx_HAL_Driver/Drivers/STM32F1xx_HAL_Driver/Inc"
)

set(SOURCES
	${SOURCES_GENERATED}
	${SOURCES_PROJECT}
)

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
  message(STATUS "dir='${dir}'")
endforeach()

add_executable(${NAME} ${SOURCES})

set_target_properties(${NAME} PROPERTIES LINK_FLAGS "-T ${PLATFORM_LINKER_SCRIPT} -specs=nano.specs -Wl,--gc-sections")

target_link_libraries(${NAME}
    core
	libs
	Drivers
)

target_generate_hex(${NAME})
target_memory_report(${NAME})
target_asm_listing(${NAME})

target_format_sources(${NAME} "${SOURCES_PROJECT}")