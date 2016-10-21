all: tests

EXTERNALS_FOLDER=external
BUILD_FOLDER=build
GTEST_FOLDER=${EXTERNALS_FOLDER}/googletest
GTEST_LIB=${GTEST_FOLDER}/build/googlemock/gtest/libgtest.a
GTEST_LIB_MAIN=${GTEST_FOLDER}/build/googlemock/gtest/libgtest_main.a

${EXTERNALS_FOLDER}:
	mkdir ${EXTERNALS_FOLDER}

${GTEST_FOLDER}: ${EXTERNALS_FOLDER}
	git clone https://github.com/google/googletest.git ${GTEST_FOLDER}

${BUILD_FOLDER}:
	mkdir ${BUILD_FOLDER}

${BUILD_FOLDER}/test: ${BUILD_FOLDER} ${GTEST_FOLDER}
	cd build; cmake ..; make

tests: build/test
	build/tests

clean:
	rm -rf build


.PHONY += all tests build
