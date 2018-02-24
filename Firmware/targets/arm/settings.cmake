set (CWARN "-Wall -Wstrict-prototypes -Wextra -Werror")
set (CXXWARN "-Wall -Wextra -Werror")
set(CTUNING "-fomit-frame-pointer -ffunction-sections -fdata-sections")
set(CMCU "-mcpu=cortex-m3 -mthumb -mfloat-abi=soft")
set(RANDOM_DEFS "-DUSE_HAL_DRIVER -DSTM32F103xB")

set(CMAKE_C_FLAGS "-std=gnu11 ${CWARN} ${CTUNING} ${CMCU} ${RANDOM_DEFS}")
set(CMAKE_CXX_FLAGS "-std=gnu++17 -Dregister=\"/**/\" -fno-exceptions -fno-rtti ${CXXWARN} ${CTUNING} ${CMCU} ${RANDOM_DEFS}")
set(CMAKE_CXX_STANDARD 17)

set(PLATFORM_LINKER_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/STM32F103C8Tx_FLASH.ld)
