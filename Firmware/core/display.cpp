#include <cstdio>
#include <cstring>
#include <storage/storage.h>

#include "HAL.h"
#include "com.h"
#include "stateManager.h"

namespace core::display {

void tick() {
    char line1[17], line2[17] = {' '};

    snprintf(line1,
        17,
        "%3d/%3d %c   %3d%%",
        static_cast<int>(temp),
        core::persistent_state.target,
        storage::stateIsSaved() ? ' ' : '*',
        static_cast<int>(power));

    if (stateManager::state == stateManager::State::InvalidConfig) {
        std::strncpy(line2, "   CONNECT PC   ", 17);
    } else if (stateManager::state == stateManager::State::TipError) {
        std::strncpy(line2, "   TIP ERROR    ", 17);
    } else if (stateManager::state == stateManager::State::Off) {
        std::strncpy(line2, "      OFF       ", 17);
    } else if (stateManager::state == stateManager::State::Sleep) {
        std::strncpy(line2, "     sleep      ", 17);
    } else if (stateManager::state == stateManager::State::InStand) {
        std::strncpy(line2, "    in stand    ", 17);
    } else {
        std::strncpy(line2, "                ", 17);
    }

    int lastBarElement = 0;
    if (core::settings.pid.max_power > 0.1) {
        lastBarElement = static_cast<int>(16.0 * power / core::settings.pid.max_power);
    }
    for (int i = 0; i < lastBarElement; ++i) {
        if (isalpha(line2[i])) {
            line2[i] = std::toupper(line2[i]);
        } else {
            line2[i] = '#';
        }
    }

    HAL::Display::write(line1, line2);
}

void setDisplaySettings() {
    HAL::Display::set(core::settings.display.backlight, core::settings.display.contrast);
}

}  // namespace core::display
