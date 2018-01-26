set(NAME arm)

add_subdirectory(Drivers)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-strict-prototypes")

include(Drivers/allowed_warnings.cmake)

set(SOURCES_GENERATED
	Inc/adc.h
	Inc/gpio.h
	Inc/i2c.h
	Inc/iwdg.h
	Inc/main.h
	Inc/stm32f1xx_hal_conf.h
	Inc/stm32f1xx_it.h
	Inc/tim.h
	Inc/usart.h

	Src/adc.c
	Src/gpio.c
	Src/i2c.c
	Src/iwdg.c
	Src/main.c
	Src/stm32f1xx_hal_msp.c
	Src/stm32f1xx_it.c
	Src/system_stm32f1xx.c
	Src/tim.c
	Src/usart.c

	startup/startup_stm32f103xb.s
)

set(SOURCES_PROJECT
	Inc/core_support.h
	Inc/encoder_hw.h
	Inc/hardwareConfig.h
	Inc/hd44780.h
	Inc/i2c_memory.h

	Src/HAL.cpp
	Src/hd44780.c
	Src/MY_main.cpp
	Src/newlib.c
	Src/encoder_hw.cpp
	Src/i2c_memory.cpp
)

include_directories(
	Inc
	Drivers/CMSIS/Device/ST/STM32F1xx/Include
	Drivers/CMSIS/Include
	Drivers/STM32F1xx_HAL_Driver/Drivers/STM32F1xx_HAL_Driver/Inc
)

set(SOURCES
	${SOURCES_GENERATED}
	${SOURCES_PROJECT}
)

add_executable(${NAME} ${SOURCES})

set_target_properties(${NAME} PROPERTIES LINK_FLAGS "-T ${PLATFORM_LINKER_SCRIPT} -Wl,--gc-sections --specs=nano.specs -lc -lnosys -u _printf_float")

target_link_libraries(${NAME}
    core
	libs
	Drivers
)

target_generate_hex(${NAME})

target_memory_report(${NAME})
target_asm_listing(${NAME})

target_format_sources(${NAME} "${SOURCES_PROJECT}")