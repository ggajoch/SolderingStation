set(NAME unit_tests)

file(GLOB SOURCES
        "*.h"
        "*.cpp"
)

# SETUP_TARGET_FOR_COVERAGE(
#     NAME testrunner_coverage                    # New target name
#     EXECUTABLE testrunner -j ${PROCESSOR_COUNT} # Executable in PROJECT_BINARY_DIR
#     DEPENDENCIES testrunner                     # Dependencies to build first
# )

add_executable(${NAME} ${SOURCES})

APPEND_COVERAGE_COMPILER_FLAGS()

SETUP_TARGET_FOR_COVERAGE(
    NAME ${NAME}_coverage                    # New target name
    EXECUTABLE ${NAME} -j ${PROCESSOR_COUNT} # Executable in PROJECT_BINARY_DIR
    DEPENDENCIES ${NAME}                     # Dependencies to build first
)

target_link_libraries(${NAME}
	libs
	core
	gtest
	gtest_main
)

target_format_sources(${NAME} "${SOURCES}")