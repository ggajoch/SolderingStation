all: style tests

EXTERNALS_FOLDER=external
BUILD_FOLDER=build
GTEST_FOLDER=${EXTERNALS_FOLDER}/googletest
GTEST_LIB=${GTEST_FOLDER}/build/googlemock/gtest/libgtest.a
GTEST_LIB_MAIN=${GTEST_FOLDER}/build/googlemock/gtest/libgtest_main.a
GTEST_REPO=https://github.com/google/googletest.git

${EXTERNALS_FOLDER}:
	mkdir ${EXTERNALS_FOLDER}

${GTEST_FOLDER}: ${EXTERNALS_FOLDER}
	@if [ -e "external/googletest" ]; then echo "Exists"; else git clone ${GTEST_REPO} ${GTEST_FOLDER}; fi
	

${BUILD_FOLDER}:
	mkdir ${BUILD_FOLDER}

${BUILD_FOLDER}/test: ${BUILD_FOLDER} ${GTEST_FOLDER}
	cd build; cmake ..; make

tests: build/test
	build/tests

clean:
	rm -rf build


${EXTERNALS_FOLDER}/cpplint.py: ${EXTERNALS_FOLDER}
	wget https://github.com/google/styleguide/raw/gh-pages/cpplint/cpplint.py -O ${EXTERNALS_FOLDER}/cpplint.py


LINTER_PARAMS=--extensions=hpp,cpp,h,c --filter=-legal/copyright,-build/include,-runtime/arrays,-runtime/printf --root=Firmware --linelength=120

style: ${EXTERNALS_FOLDER}/cpplint.py
	find . -not -path "./build/*" -not -path "./${EXTERNALS_FOLDER}/*" -type f \( -iname \*.h -o -iname \*.hpp -o -iname \*.h -o -iname \*.cpp \) | xargs python2 ${EXTERNALS_FOLDER}/cpplint.py $(LINTER_PARAMS)

.PHONY += all tests build
