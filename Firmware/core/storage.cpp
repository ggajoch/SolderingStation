#include "storage.h"
#include "HAL.h"
#include "config.h"
#include "display.h"
#include "tempSensor.h"
#include "timer.h"

namespace core {
namespace storage {

Settings actualSettings() {
    return {.pidParams = core::pid.params,
        .tipParams = core::tempSensor::params,
        .contrast = core::display::contrast,
        .backlight = core::display::backlight};
}

State actualState() {
    return {.targetTemperature = core::target};
}

Settings settingsInMemory;
State stateInMemory;

void read() {
    HAL::Memory::getSettings(&settingsInMemory);
    HAL::Memory::getState(&stateInMemory);
    core::target = stateInMemory.targetTemperature;
    core::pid.params = settingsInMemory.pidParams;
    core::tempSensor::params = settingsInMemory.tipParams;
    core::display::setContrast(settingsInMemory.contrast);
    core::display::setBacklight(settingsInMemory.backlight);
}

// this function should be invoked on every tick
void tick() {
    static std::chrono::milliseconds saveDataTimePoint{0};
    static bool saved = true;

    State nowState = actualState();
    if (nowState != stateInMemory) {
        saveDataTimePoint = timer::now() + config::timeToSaveDataToMemory;
        stateInMemory = nowState;
        saved = false;
    }

    if (!saved && timer::now() > saveDataTimePoint) {
        saved = true;
        HAL::Memory::storeState(&stateInMemory);
    }

    Settings nowSettings = actualSettings();
    if (nowSettings != settingsInMemory) {
        settingsInMemory = nowSettings;
        HAL::Memory::storeSettings(&settingsInMemory);
    }
}

}  // namespace storage
}  // namespace core
