#include "storage.h"
#include "HAL.h"
#include "config.h"
#include "display.h"
#include "sleepManager.h"
#include "tempSensor.h"
#include "timer.h"
#include "com.h"

#include "settings.h"
#include "persistent_state.h"
#include "state.h"

namespace core {
Settings settings;
PersistentState persistent_state;

namespace storage {

Settings settingsInMemory;
PersistentState stateInMemory;

void read() {
    auto settings = getSettings();
    if (!settings) {
        com::printf("Incorrect setting in memory, fallback to defaults\n");
        core::sleepManager::configurationCorrectState = false;

        core::settings.pidParams = {.Kp = 0.0, .Ki = 0.0, .Kd = 0.0};
        core::settings.tipParams = {.offset = 0, .gain = 0.0};
        core::settings.contrast = 27.5;
        core::settings.backlight = 100;
    } else {
        com::printf("Setting from memory loaded\n");
        core::sleepManager::configurationCorrectState = true;
        core::settings = *settings;

        settingsInMemory = *settings;
    }

    core::display::setContrast(core::settings.contrast);
    core::display::setBacklight(core::settings.backlight);

    auto state = getState();
    if (state) {
        stateInMemory = *state;
    } else {
        stateInMemory.target = 0.0f;
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

    if (!saved && timer::now() > saveDataTimePoint) {
        saved = true;
        saveState(stateInMemory);
    }

    if (core::settings != settingsInMemory && core::sleepManager::configurationCorrectState) {
        settingsInMemory = core::settings;
        storeSettings(core::settings);
    }
}

}  // namespace storage
}  // namespace core
