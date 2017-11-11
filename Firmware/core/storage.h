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

struct Settings {
    PID::Params pidParams;
    tempSensor::Params tipParams;
    float contrast;
    float backlight;

    gsl::span<std::uint8_t> asArrayView() {
        return gsl::make_span(reinterpret_cast<std::uint8_t*>(this), sizeof(*this));
    }

    bool operator==(const Settings& rhs) {
        return std::memcmp(reinterpret_cast<char*>(this), reinterpret_cast<const char*>(&rhs), sizeof(Settings)) == 0;
    }

    bool operator!=(const Settings& rhs) {
        return !(*this == rhs);
    }
};

struct State {
    float targetTemperature;

    gsl::span<std::uint8_t> asArrayView() {
        return gsl::make_span(reinterpret_cast<std::uint8_t*>(this), sizeof(*this));
    }

    bool operator==(const State& rhs) {
        return std::memcmp(reinterpret_cast<char*>(this), reinterpret_cast<const char*>(&rhs), sizeof(State)) == 0;
    }

    bool operator!=(const State& rhs) {
        return !(*this == rhs);
    }
};

void read();

bool stateIsSaved();
// this function should be invoked on every tick
void tick();

}  // namespace storage
}  // namespace core

#endif  // CORE_STORAGE_H_
