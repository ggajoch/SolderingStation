#ifndef CORE_STORAGE_H_
#define CORE_STORAGE_H_

#include <cstdint>
#include <cstring>
#include <gsl/span>
#include "core.h"
#include "pid.h"
#include "tempSensor.h"

namespace core {
namespace storage {

struct Elements {
    float targetTemperature;
    PID::Params pidParams;
    tempSensor::Params tipParams;
    float contrast;
    float backlight;

    gsl::span<std::uint8_t> asArrayView() {
        return gsl::make_span(reinterpret_cast<std::uint8_t*>(this), sizeof(*this));
    }

    bool operator==(const Elements& rhs) {
        return std::memcmp(reinterpret_cast<char*>(this), reinterpret_cast<const char*>(&rhs), sizeof(Elements)) == 0;
    }

    bool operator!=(const Elements& rhs) {
        return !(*this == rhs);
    }
};

void read();

// this function should be invoked on every tick
void tick();

}  // namespace storage
}  // namespace core

#endif  // CORE_STORAGE_H_
