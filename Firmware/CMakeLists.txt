cmake_minimum_required(VERSION 3.5)

project(tests)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++1z")

add_subdirectory(external/googletest)

set(CORE_FILES core/Tip.cpp core/Controller.cpp)
set(TEST_FILES tests/tempScaling.cpp tests/Average.cpp)
set(TARGET_FILES )
set(LIBS_FILES )

set(SOURCE_FILES ${CORE_FILES} ${TEST_FILES} ${TARGET_FILES} ${LIBS_FILES})

include_directories(core libs)


add_executable(tests ${SOURCE_FILES})

target_link_libraries(tests gtest gtest_main)
