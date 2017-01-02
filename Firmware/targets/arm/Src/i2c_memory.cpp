#include "i2c_memory.h"

std::experimental::optional<i2cMemorySettings> i2cMemoryReadSettings() {
    static constexpr i2cMemorySettings settings = {
        .pidParams = {.Kp = 3.0, .Ki = 3.0, .Kd = 0}, .tipParams = {.offset = 20, .gain = 0.11}, .contrast = 27.5, .backlight = 100};

    return settings;
}
