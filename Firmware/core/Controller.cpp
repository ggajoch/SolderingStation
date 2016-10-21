#include "Controller.h"

#include "HAL.h"
#include "config.h"

#include "Average.h"
#include "Tip.h"
#include "pid.h"

namespace core {

class Controller {
 public:
    libs::Average<float, tempAverages> temperatureAverage;
    Tip tip;
    PID pid;

    void setup() {
        HAL::Tip::setHeating(0);
    }

    void loop() {
        HAL::Tip::setHeating(0);
        // [TODO]: delay?

        for (int i = 0; i < temperatureAverage.size(); ++i) {
            uint16_t raw_reading = HAL::Tip::readRaw();
            float temperature = tip.getTemperature(raw_reading);
            temperatureAverage.put(temperature);
        }

        float power = pid.tick(temperatureAverage.get());

        HAL::Tip::setHeating(power);
    }
};

};  // namespace core
