#pragma once

#include "storage/base.h"

namespace core {

struct Settings : base::Pod<Settings> {
    struct PidParams {
        float Kp, Ki, Kd;
        uint16_t max_power;
    } __attribute__((packed));

    struct TipParams {
        float offset;
        float gain;
        uint16_t max_safe_temperature;
    } __attribute__((packed));

    struct Display {
        uint8_t contrast;
        uint8_t backlight;
    } __attribute__((packed));

    struct Timeouts {
        uint8_t sleep;
        uint8_t off;
    } __attribute__((packed));

    TipParams tip;
    Display display;
    PidParams pid;
    Timeouts timeouts;

    uint16_t sleep_temperature;
    uint16_t stand_temperature;

    uint32_t padding;
} __attribute__((packed));

static_assert(sizeof(Settings) == 36, "Verify size!");

extern Settings settings;

}  // namespace core
