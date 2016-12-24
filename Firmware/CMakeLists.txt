cmake_minimum_required(VERSION 3.1)

include(toolchain.cmake)
project(SolderingStation C CXX ASM)

#set(CMAKE_BUILD_TYPE None)

set(TARGET_PLATFORM "arm" CACHE STRING "Target platform")
set(PLATFORM_PATH ${CMAKE_CURRENT_SOURCE_DIR}/targets/${TARGET_PLATFORM})
if(NOT EXISTS ${PLATFORM_PATH})
  message(FATAL_ERROR "Target platform ${TARGET_PLATFORM} does not exist")
endif()

set(CMAKE_C_FLAGS "-std=gnu11 -Os ${CWARN} ${CTUNING} ${CMCU} ${RANDOM_DEFS}" CACHE STRING "compile flags" FORCE)
set(CMAKE_CXX_FLAGS "-std=gnu++1z -Os -fno-exceptions -fno-rtti ${CXXWARN} ${CTUNING} ${CMCU} ${RANDOM_DEFS}" CACHE STRING "compile flags" FORCE)

include(${PLATFORM_PATH}/settings.cmake)

message(STATUS "X compiler from ${CMAKE_C_COMPILER} with flags ${CMAKE_CXX_FLAGS}")

set(OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/build/${TARGET_PLATFORM})
set(REPORTS_PATH ${OUTPUT_PATH}/reports)

file(MAKE_DIRECTORY ${OUTPUT_PATH})


set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_PATH}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_PATH}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_PATH}/bin)




message(STATUS "Using C compiler from ${CMAKE_C_COMPILER}")
message(STATUS "Target platform ${TARGET_PLATFORM}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "X compiler from ${CMAKE_C_COMPILER} with flags ${CMAKE_CXX_FLAGS}")
message(STATUS "Build type ${CMAKE_BUILD_TYPE}")

add_subdirectory(libs)
add_subdirectory(core)
add_subdirectory(${PLATFORM_PATH})