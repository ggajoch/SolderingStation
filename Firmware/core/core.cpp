#include <cstring>
#include "array_view.h"
#include "Average.h"

#include "HAL.h"
#include "config.h"
#include "core.h"
#include "com.h"
#include "timer.h"
#include "storage.h"
#include "display.h"
#include "sleepManager.h"
#include "encoder.h"

namespace core {

PID pid;
float temp, target, power;


void setup() {
    storage::read();
    HAL::Tip::setHeating(0);
    HAL::Com::setCallback(com::rxCommandCallback);
    HAL::Encoder::setButtonCallback(sleepManager::encoderCallback);
}

void tick() {
    sleepManager::tick();
    encoder::tick();

    HAL::Tip::setHeating(0);
    HAL::delay(5);

    temp = tempSensor::getAveragedTemperature();
    power = pid.tick(temp);
    HAL::Tip::setHeating(power);

    com::tick();
    display::tick();
    timer::tick();
    storage::tick();
}

};  // namespace core
