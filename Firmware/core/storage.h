#ifndef SOLDERINGSTATION_STORAGE_H
#define SOLDERINGSTATION_STORAGE_H

#include <cstring>
#include "core.h"
#include "pid.h"
#include "tip.h"

namespace core {

namespace storage {

struct Elements {
    float targetTemperature;
    PID::Params pidParams;
    Tip::Params tipParams;
    float contrast;
    float backlight;

    libs::array_view<uint8_t> asArrayView() {
        return libs::make_array_view(reinterpret_cast<uint8_t *>(this), sizeof(*this));
    }

    bool operator==(const Elements & rhs) {
        return std::memcmp(reinterpret_cast<char*>(this),
                           reinterpret_cast<const char*>(&rhs),
                           sizeof(Elements)) == 0;
    }

    bool operator!=(const Elements & rhs) {
        return !(*this == rhs);
    }
};


void read();

// this function should be invoked on every tick
void save();

};



};  // namespace core

#endif  // SOLDERINGSTATION_STORAGE_H
