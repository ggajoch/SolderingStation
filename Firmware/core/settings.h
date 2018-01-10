#pragma once

#include "storage/base.h"

namespace core {

struct Settings : base::Pod<Settings> {
    struct PidParams {
        float Kp, Ki, Kd;
        float max_power;
    };

    struct TipParams {
        float offset, gain;
    };

    struct Display {
        float contrast;
        float backlight;
    };

    PidParams pidParams;
    TipParams tipParams;
    Display display;
    float _padding_;
} __attribute__((packed));

extern Settings settings;

}  // namespace core
