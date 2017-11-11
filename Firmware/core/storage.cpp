#include "storage.h"
#include "HAL.h"
#include "config.h"
#include "display.h"
#include "sleepManager.h"
#include "tempSensor.h"
#include "timer.h"
#include "com.h"

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
    auto settings = HAL::Memory::getSettings();
    if (!settings) {
        com::printf("Incorrect setting in memory, fallback to default\n");
        core::sleepManager::configStateSet(false);
        static constexpr core::PID::Params pidParams = {.Kp = 0.0, .Ki = 0.0, .Kd = 0.0};
        static constexpr core::tempSensor::Params tipParams = {.offset = 0, .gain = 0.0};
        core::pid.params = pidParams;
        core::tempSensor::params = tipParams;
        core::display::setContrast(27.5);
        core::display::setBacklight(100.0);
    } else {
        com::printf("Setting from memory loaded\n");
        core::sleepManager::configStateSet(true);
        settingsInMemory = *settings;
        core::pid.params = settingsInMemory.pidParams;
        core::tempSensor::params = settingsInMemory.tipParams;
        core::display::setContrast(settingsInMemory.contrast);
        core::display::setBacklight(settingsInMemory.backlight);
    }

    auto state = HAL::Memory::getState();
    if (state) {
        stateInMemory = *state;
    } else {
        stateInMemory.targetTemperature = 0;
    }
    core::target = stateInMemory.targetTemperature;
}

bool saved = true;

bool stateIsSaved() {
    return saved;
}
// this function should be invoked on every tick
void tick() {
    static std::chrono::milliseconds saveDataTimePoint{0};

    State nowState = actualState();
    if (nowState != stateInMemory) {
        saveDataTimePoint = timer::now() + config::timeToSaveDataToMemory;
        stateInMemory = nowState;
        saved = false;
    }

    if (!saved && timer::now() > saveDataTimePoint) {
        saved = true;
        HAL::Memory::storeState(stateInMemory);
    }

    Settings nowSettings = actualSettings();
    if (nowSettings != settingsInMemory) {
        settingsInMemory = nowSettings;
        HAL::Memory::storeSettings(settingsInMemory);
    }
}

}  // namespace storage
}  // namespace core
