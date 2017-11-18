#include "storage.h"
#include "com.h"
#include "config.h"
#include "stateManager.h"
#include "timer.h"

#include "persistent_state.h"
#include "settings.h"
#include "state.h"

namespace core {
namespace storage {

Settings settingsInMemory;
PersistentState stateInMemory;

void read() {
    auto settings = getSettings();
    if (!settings) {
        com::printf("Incorrect setting in memory, fallback to defaults\n");
        core::stateManager::configuration_correct = false;

        core::settings.pidParams = {.Kp = 0.0, .Ki = 0.0, .Kd = 0.0};
        core::settings.tipParams = {.offset = 0, .gain = 0.0};
        core::settings.display.contrast = 27.5;
        core::settings.display.backlight = 100;
    } else {
        com::printf("Setting from memory loaded\n");
        core::stateManager::configuration_correct = true;
        core::settings = *settings;

        settingsInMemory = *settings;
    }

    core::display::setDisplaySettings(core::settings.display.backlight, core::settings.display.contrast);

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

    if (core::settings != settingsInMemory && core::stateManager::configuration_correct) {
        settingsInMemory = core::settings;
        storeSettings(core::settings);
    }
}

}  // namespace storage
}  // namespace core
