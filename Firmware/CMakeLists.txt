cmake_minimum_required(VERSION 3.3)
project(solder)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -D__IO=volatile -DSTM32F103xB -DUSE_HAL_DRIVER")


file(GLOB_RECURSE CubeHAL Drivers/*.c Drivers/*.s)

file(GLOB_RECURSE Sources Src/*.c Src/*.cpp Libs/*.c Libs/*.cpp)


include_directories(Inc
        Drivers/CMSIS/Device/Include Drivers/CMSIS/Include
        Drivers/STM32F1xx_HAL_Driver/Inc Drivers/STM32F1xx_HAL_Driver/Inc/Legacy
        Libs/LCD/inc)

set(SOURCE_FILES ${CubeHAL} ${Sources} Inc/USART.h Libs/LCD/inc/LCDTransaction.h Inc/ArrayHolder.h)
add_executable(solder ${SOURCE_FILES})