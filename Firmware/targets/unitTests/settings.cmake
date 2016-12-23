#set (CWARN "-Wall -Wstrict-prototypes -Wextra -Werror")
#set (CXXWARN "-Wall -Wextra -Werror")
set (CTUNING "-ggdb -pedantic -fomit-frame-pointer -ffunction-sections -fdata-sections")
set (CMAKE_C_FLAGS "-std=gnu11 ${CWARN} ${CTUNING} ${CMCU} ${CCOVERAGE}")
set (CMAKE_CXX_FLAGS "-std=gnu++1y -fno-exceptions ${CXXWARN} ${CTUNING} ${CMCU} ${CCOVERAGE}")
set (CMAKE_CXX_STANDARD 14)

set(DEBUG_COMP_OPTIONS "-DDEBUG -Og -g")
set(RELEASE_COMP_OPTIONS "-DNDEBUG -O2")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

set(CMAKE_C_FLAGS_DEBUG ${DEBUG_COMP_OPTIONS})
set(CMAKE_CXX_FLAGS_DEBUG ${DEBUG_COMP_OPTIONS})
set(CMAKE_C_FLAGS_RELEASE ${RELEASE_COMP_OPTIONS})
set(CMAKE_CXX_FLAGS_RELEASE ${RELEASE_COMP_OPTIONS})
