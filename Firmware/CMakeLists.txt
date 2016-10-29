cmake_minimum_required(VERSION 3.5)

project(SolderingStation)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++1z")

add_subdirectory(external/googletest)

include_directories(libs)
file(GLOB LIBS_FILES
        "libs/*.h"
        "libs/*.cpp"
        )

include_directories(core)
file(GLOB CORE_FILES
        "core/*.h"
        "core/*.cpp"
        )


file(GLOB SIMULATION_EVENTS_FILES
        "targets/simulation-events/*.h"
        "targets/simulation-events/*.cpp"
        )

add_executable(simulation_events ${SIMULATION_EVENTS_FILES} ${CORE_FILES} ${LIBS_FILES})
target_link_libraries(simulation_events gtest)



file(GLOB UNITTESTS_FILES
        "targets/unitTests/*.h"
        "targets/unitTests/*.cpp"
        )

add_executable(unitTests ${CORE_FILES} ${UNITTESTS_FILES} ${LIBS_FILES})
target_link_libraries(unitTests gtest gtest_main)