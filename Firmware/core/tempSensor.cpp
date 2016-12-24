#include "Average.h"
#include "tempSensor.h"
#include "config.h"
#include "HAL.h"

namespace core {
namespace tempSensor {

Params params;

float calcTemperature(int raw_reading) {
    return params.offset + params.gain*static_cast<float>(raw_reading);
}

float getAveragedTemperature() {
    libs::Average<float, config::tempAverages> temperatureAverage;

    for (int i = 0; i < temperatureAverage.size(); ++i) {
        uint16_t raw_reading = HAL::Tip::readRaw();
        float temperature = calcTemperature(raw_reading);
        temperatureAverage.put(temperature);
    }
    return temperatureAverage.get();
}

}  // namespace tempSensor
}  // namespace core
