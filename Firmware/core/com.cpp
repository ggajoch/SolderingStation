#include <cstdio>
#include <stdarg.h>
#include "HAL.h"
#include "CLI.h"
#include "core.h"

namespace core {
namespace com {

int printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    static char buf[100];
    int n = vsprintf(buf, format, args);
    HAL::Com::puts(buf);

    va_end(args);
    return n;
}

char * rxCommandPtr = nullptr;
void rxCommandCallback(char * data) {
    rxCommandPtr = data;
}

void tick() {
    core::com::printf("TICK %.2f %.2f %.2f %.2f\n", core::temp, core::target, core::power, core::pid.integral);

    if (rxCommandPtr != nullptr) {
        libs::CLI::parse_line(rxCommandPtr);
        rxCommandPtr = nullptr;
    }
    HAL::Com::puts("*\n");
}

}  // namespace com
}  // namespace core
