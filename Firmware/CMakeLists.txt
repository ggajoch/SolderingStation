cmake_minimum_required(VERSION 3.5)

project(tests)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++1z")

add_subdirectory(external/googletest)

set(CORE_FILES core/Controller.cpp core/pid.h core/HAL.h core/Tip.h)
set(TEST_FILES tests/tempScaling.cpp tests/Average.cpp tests/array_view.cpp tests/Controller.cpp tests/PID.cpp tests/plantModel.cpp tests/PIDforTip.cpp)
set(TARGET_FILES target/mock/HALmock.cpp tests/plantModel.h)
set(LIBS_FILES )

set(SOURCE_FILES ${CORE_FILES} ${TEST_FILES} ${TARGET_FILES} ${LIBS_FILES} core/config.h)

include_directories(core libs target/mock)


add_executable(tests ${SOURCE_FILES})

target_link_libraries(tests gtest gtest_main)
