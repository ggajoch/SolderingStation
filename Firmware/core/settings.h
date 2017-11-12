#ifndef SOLDERINGSTATION_SETTINGS_H
#define SOLDERINGSTATION_SETTINGS_H

#include "storage/base.h"

namespace core {

struct Settings : base::Pod<Settings> {
    struct PidParams {
        float Kp, Ki, Kd;
    };

    struct TipParams {
        float offset, gain;
    };

    PidParams pidParams;
    TipParams tipParams;
    float contrast;
    float backlight;
} __attribute__((packed));

extern Settings settings;

}

#endif //SOLDERINGSTATION_SETTINGS_H
