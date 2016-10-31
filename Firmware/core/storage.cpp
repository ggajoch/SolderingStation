#include "config.h"
#include "storage.h"
#include "timer.h"

namespace core {
namespace storage {

Elements actualState() {
    return {
            .targetTemperature = core::target,
            .pidParams = core::pid.params,
            .tipParams = core::tip.params,
            .contrast = 0,
            .backlight = 100
    };
}


Elements inMemory;

void read() {
    HAL::Memory::get(inMemory.asArrayView());
    core::target = inMemory.targetTemperature;
    core::pid.params = inMemory.pidParams;
    core::tip.params = inMemory.tipParams;
}

// this function should be invoked on every tick
void save() {
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


};  // namespace core
};  // namespace storage
