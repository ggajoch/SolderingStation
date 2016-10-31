#include <cstring>
#include "array_view.h"
#include "Average.h"

#include "config.h"
#include "core.h"
#include "com.h"
#include "timer.h"
#include "storage.h"

namespace core {

libs::Average<float, config::tempAverages> temperatureAverage;
Tip tip;
PID pid;
float temp, target, power;


void setup() {
    storage::read();
    HAL::Tip::setHeating(0);
    HAL::Com::setCallback(com::rxCommandCallback);
}

void tick() {
    HAL::Tip::setHeating(0);
    tip.tick();
    if (tip.inStand) {
        pid.target = 0;
    } else {
        pid.target = target;
    }

    HAL::delay(5);

    for (int i = 0; i < temperatureAverage.size(); ++i) {
        uint16_t raw_reading = HAL::Tip::readRaw();
        float temperature = tip.getTemperature(raw_reading);
        temperatureAverage.put(temperature);
    }
    temp = temperatureAverage.get();

    power = pid.tick(temp);
    HAL::Tip::setHeating(power);


    core::com::printf("TICK %.2f %.2f %.2f %.2f\n", temp, target, power, pid.integral);
    com::dispatchCommand();

    char display[2][16];

    sprintf(display[0], "%3d/%3d     %3d%%",
            static_cast<int>(temp),
            static_cast<int>(target),
            static_cast<int>(power));


    if (tip.inStand) {
        std::memcpy(display[1], "     STAND      ", 16);
    } else {
        int lastBarElement = static_cast<int>(16.0 * power / 100.0);
        for (int i = 0; i < 16; ++i) {
            display[1][i] = (i < lastBarElement) ? '#' : ' ';
        }
    }

    HAL::Display::write(display);

    timer::tick();
    storage::save();
}

};  // namespace core
