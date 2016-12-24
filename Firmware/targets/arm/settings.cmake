#set (CWARN "-Wall -Wstrict-prototypes -Wextra -Werror")
#set (CXXWARN "-Wall -Wextra -Werror")
set(CTUNING "-ggdb -pedantic -fomit-frame-pointer -ffunction-sections -fdata-sections")
set(CMCU "-mcpu=cortex-m3 -mthumb -mfloat-abi=soft")
set(RANDOM_DEFS "-D__weak=__attribute__((weak)) -D__packed=__attribute__((__packed__)) -DUSE_HAL_DRIVER -DSTM32F103xB ")

set(CMAKE_C_FLAGS "-std=gnu11 -Os ${CWARN} ${CTUNING} ${CMCU} ${RANDOM_DEFS} -nostdinc++" CACHE STRING "compile flags" FORCE)
set(CMAKE_CXX_FLAGS "-std=gnu++1z -Os -fno-exceptions -fno-rtti ${CXXWARN} ${CTUNING} ${CMCU} ${RANDOM_DEFS} -nostdinc++" CACHE STRING "compile flags" FORCE)
set(CMAKE_CXX_STANDARD 14)

set(PLATFORM_LINKER_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/STM32F103C8Tx_FLASH.ld)