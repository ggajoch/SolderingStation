#include "config.h"
#include "storage.h"
#include "timer.h"
#include "tempSensor.h"
#include "HAL.h"
#include "display.h"

namespace core {
namespace storage {

Elements actualState() {
    return {
            .targetTemperature = core::target,
            .pidParams = core::pid.params,
            .tipParams = core::tempSensor::params,
            .contrast = core::display::contrast,
            .backlight = core::display::backlight
    };
}


Elements inMemory;

void read() {
    HAL::Memory::get(inMemory.asArrayView());
    core::target = inMemory.targetTemperature;
    core::pid.params = inMemory.pidParams;
    core::tempSensor::params = inMemory.tipParams;
    core::display::setContrast(inMemory.contrast);
    core::display::setBacklight(inMemory.backlight);
}

// this function should be invoked on every tick
void tick() {
    static std::chrono::milliseconds saveDataTimePoint;
    static bool saved = true;

    Elements now = actualState();
    if (now != inMemory) {
        saveDataTimePoint = timer::now() + config::timeToSaveDataToMemory;
        inMemory = now;
        saved = false;
    }

    if (!saved && timer::now() > saveDataTimePoint) {
        saved = true;
        HAL::Memory::store(inMemory.asArrayView());
    }
}


};  // namespace storage
};  // namespace core
