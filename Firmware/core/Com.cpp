#include <cstdio>
#include <stdarg.h>

#include "HAL.h"

namespace HAL {
namespace Com {

int printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    static char buf[100];
    int n = vsprintf(buf, format, args);
    puts(buf);

    va_end(args);
    return n;
}

};  // namespace Com
};  // namespace HAL
