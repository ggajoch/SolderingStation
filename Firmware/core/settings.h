#pragma once

#include "storage/base.h"

namespace core {

struct Settings : base::Pod<Settings> {
    struct PidParams {
        float Kp, Ki, Kd;
        uint16_t max_power;
    } __attribute__((packed));

    struct TipParams {
        float offset, gain;
    };

    struct Display {
        float contrast;
        float backlight;
    };

    struct Timeouts {
        uint8_t sleep;
        uint8_t off;
    };

    TipParams tipParams;
    Display display;
    PidParams pidParams;
    Timeouts timeouts;

    uint16_t sleep_temperature;
    uint16_t stand_temperature;
} __attribute__((packed));

static_assert(sizeof(Settings) == 36, "Verify size!");

extern Settings settings;

}  // namespace core
