#ifndef SOLDERINGSTATION_SETTINGS_H
#define SOLDERINGSTATION_SETTINGS_H

#include <cstring>
#include <gsl/span>

namespace core {

struct Settings {
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

    gsl::span <std::uint8_t> as_span() {
        return gsl::make_span(reinterpret_cast<std::uint8_t *>(this), sizeof(*this));
    }

    bool operator==(const Settings &rhs) const {
        return std::memcmp(reinterpret_cast<const char *>(this), reinterpret_cast<const char *>(&rhs), sizeof(Settings))
            == 0;
    }

    bool operator!=(const Settings &rhs) const {
        return !(*this == rhs);
    }
};

extern Settings settings;

}

#endif //SOLDERINGSTATION_SETTINGS_H
