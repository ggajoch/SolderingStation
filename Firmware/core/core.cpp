#include <cstring>
#include "Average.h"

#include "HAL.h"
#include "com.h"
#include "commands.h"
#include "config.h"
#include "core.h"
#include "display.h"
#include "encoder.h"
#include "sleepManager.h"
#include "storage.h"
#include "timer.h"

namespace core {

PID pid;
float temp, target, power;

void setup() {
    commands::setup();
    storage::read();
    HAL::Tip::setHeating(0);
}

void tick() {
    sleepManager::tick();
    encoder::tick();

    HAL::Tip::setHeating(0);
    HAL::delay(5);

    temp = tempSensor::getAveragedTemperature();
    power = pid.tick(temp - 0.5);  // 0.5 to follow temperature between full degrees
    HAL::Tip::setHeating(power);

    com::tick();
    display::tick();
    timer::tick();
    storage::tick();
}

}  // namespace core
