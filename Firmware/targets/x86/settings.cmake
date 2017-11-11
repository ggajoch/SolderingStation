set(CWARN "-Wall -Wstrict-prototypes -Wextra")
set(CXXWARN "-Wall -Wextra")

set(CMAKE_C_FLAGS "-std=gnu11 ${CWARN} ${CMCU}")
set(CMAKE_CXX_FLAGS "-std=gnu++14 ${CXXWARN} ${CMCU}")
set(CMAKE_CXX_STANDARD 14)
