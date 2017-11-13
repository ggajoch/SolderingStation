#include <storage/storage.h>
#include "HAL.h"
#include "com.h"
#include "commands.h"
#include "encoder.h"
#include "stateManager.h"
#include "timer.h"

namespace core {

PID pid;
float temp, power;
PersistentState persistent_state;
Settings settings;

void setup() {
    HAL::Tip::setHeating(0);
    commands::setup();
    storage::read();
}

void tick() {
    com::tick();
    stateManager::tick();
    encoder::tick();

    HAL::Tip::setHeating(0);
    HAL::delay(5);

    temp = tempSensor::getAveragedTemperature();
    power = pid.tick(temp);
    HAL::Tip::setHeating(power);

    timer::tick();
    storage::tick();
    display::tick();
}

}  // namespace core
