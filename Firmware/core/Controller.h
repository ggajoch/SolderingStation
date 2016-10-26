#ifndef CORE_CONTROLLER_H_
#define CORE_CONTROLLER_H_

#include "HAL.h"
#include "config.h"

#include "Average.h"
#include "Tip.h"
#include "pid.h"
#include "Com.h"

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
        HAL::Com::printf("TICK %.2f %.2f %.2f %.2f\n", temperatureAverage.get(), pid.target, lastPower, pid.integral);
        HAL::delay(5);

        for (int i = 0; i < temperatureAverage.size(); ++i) {
            uint16_t raw_reading = HAL::Tip::readRaw();
            float temperature = tip.getTemperature(raw_reading);
            temperatureAverage.put(temperature);
        }

        lastPower = pid.tick(temperatureAverage.get());

        HAL::Tip::setHeating(lastPower);
    }

 private:
    float lastPower;
};

extern Controller controller;

};  // namespace core

#endif  // CORE_CONTROLLER_H_
