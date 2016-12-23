cmake_minimum_required(VERSION 3.1)

#include(toolchain.cmake)
#include(utils/functions.cmake)

project(SolderingStation C CXX ASM)

set(TARGET_PLATFORM "unitTests" CACHE STRING "Target platform")
set(PLATFORM_PATH ${CMAKE_CURRENT_SOURCE_DIR}/targets/${TARGET_PLATFORM})

set(OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/build/${TARGET_PLATFORM})
set(REPORTS_PATH ${OUTPUT_PATH}/reports)

file(MAKE_DIRECTORY ${OUTPUT_PATH})

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_PATH}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_PATH}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_PATH}/bin)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
else()
    set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "" FORCE)
endif()

if(EXISTS ${PLATFORM_PATH})
else()
  message(FATAL_ERROR "Target platform ${TARGET_PLATFORM} does not exist")
endif()

include(${PLATFORM_PATH}/settings.cmake)



message(STATUS "Using C compiler from ${CMAKE_C_COMPILER}")
message(STATUS "Target platform ${TARGET_PLATFORM}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Code coverage: ${ENABLE_COVERAGE}")

add_subdirectory(libs)
add_subdirectory(core)
add_subdirectory(${PLATFORM_PATH})