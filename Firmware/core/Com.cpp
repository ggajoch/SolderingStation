#include <cstdio>
#include <stdarg.h>
#include <experimental/optional>
#include "HAL.h"
#include "CLI.h"

namespace core {
namespace com {

int printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    static char buf[100];
    int n = vsprintf(buf, format, args);
    puts(buf);

    va_end(args);
    return n;
}

std::experimental::optional<char *> rxCommandPtr = {};
void rxCommandCallback(char * data) {
    rxCommandPtr = data;
}

void dispatchCommand() {
    if (rxCommandPtr) {
        libs::CLI::parse_line(*rxCommandPtr);
        rxCommandPtr = {};
    }
    HAL::Com::puts("*");
}

};  // namespace com
};  // namespace core
