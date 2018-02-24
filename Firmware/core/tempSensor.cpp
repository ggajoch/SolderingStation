#include "tempSensor.h"
#include "Average.h"
#include "HAL.h"
#include "config.h"
#include "core.h"
#include "settings.h"

namespace core::tempSensor {

float calcTemperature(int raw_reading) {
    return settings.tip.offset + settings.tip.gain * static_cast<float>(raw_reading);
}

float getAveragedTemperature() {
    libs::Average<float, config::tempAverages> temperatureAverage;

    for (uint16_t i = 0; i < temperatureAverage.size(); ++i) {
        uint16_t raw_reading = HAL::Tip::readRaw();
        float temperature = calcTemperature(raw_reading);
        temperatureAverage.put(temperature);
    }
    return temperatureAverage.get();
}

bool temperatureInLimits() {
    return (core::temp < settings.tip.max_safe_temperature);
}

}  // namespace core::tempSensor
