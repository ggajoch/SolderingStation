#include "storage.h"
#include "com.h"
#include "config.h"
#include "stateManager.h"
#include "timer.h"

#include "settings.h"
#include "state.h"

namespace core::storage {

Settings settingsInMemory;
PersistentState stateInMemory;

void read() {
    auto settings = getSettings();
    if (!settings) {
        com::printf("Incorrect setting in memory, fallback to defaults\n");

        core::settings.pid = {.Kp = 0.0, .Ki = 0.0, .Kd = 0.0, .max_power = 0};
        core::settings.tip = {.offset = 0, .gain = 0.0, .max_safe_temperature = 0};
        core::settings.display.contrast = 27;
        core::settings.display.backlight = 100;
        core::settings.sleep_temperature = 0;
        core::settings.stand_temperature = 0;

        core::stateManager::state = core::stateManager::State::InvalidConfig;
    } else {
        com::printf("Setting from memory loaded\n");
        core::settings = *settings;

        settingsInMemory = *settings;

        core::stateManager::state = core::stateManager::State::Off;
    }

    core::display::setDisplaySettings();

    auto state = getState();
    if (state) {
        stateInMemory = *state;
    } else {
        stateInMemory.target = 0;
    }
    core::persistent_state = stateInMemory;
}

bool saved = true;

bool stateIsSaved() {
    return saved;
}

void tick() {
    static std::chrono::milliseconds saveDataTimePoint{0};

    if (core::persistent_state != stateInMemory) {
        saveDataTimePoint = timer::now() + config::timeToSaveDataToMemory;
        stateInMemory = core::persistent_state;
        saved = false;
    }

    if (!saved && timer::now() >= saveDataTimePoint) {
        saved = true;
        saveState(stateInMemory);
    }

    if (core::settings != settingsInMemory && core::stateManager::state != core::stateManager::State::InvalidConfig) {
        settingsInMemory = core::settings;
        storeSettings(core::settings);
    }
}

}  // namespace core::storage
