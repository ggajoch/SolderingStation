#include <cstdarg>
#include <cstdio>

#include "CLI.h"
#include "HAL.h"
#include "core.h"
#include "storage/persistent_state.h"

namespace core {
namespace com {

int printf(const char* format, ...) {
    va_list args;
    va_start(args, format);

    static char buf[100];
    int n = std::vsprintf(buf, format, args);
    HAL::Com::puts(buf);

    va_end(args);
    return n;
}

char* rxCommandPtr = nullptr;

void tick() {
    if (rxCommandPtr != nullptr) {
        libs::CLI::parse_line(rxCommandPtr);
        rxCommandPtr = nullptr;
    }

    core::com::printf("TICK %.2f %d %.2f %.2f\n", core::temp, core::persistent_state.target, core::power, core::pid.integral);
}

}  // namespace com
}  // namespace core

namespace HAL {
namespace Com {

void handler(char* data) {
    core::com::rxCommandPtr = data;
}

}  // namespace Com
}  // namespace HAL