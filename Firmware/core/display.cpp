#include <cstdio>
#include <cstring>
#include <storage/storage.h>

#include "HAL.h"
#include "stateManager.h"
#include "com.h"
#include "storage/persistent_state.h"

namespace core {
namespace display {

void tick() {
    char display[2][16];

    std::sprintf(display[0],
        "%3d/%3d %c   %3d%%",
        static_cast<int>(temp),
        core::persistent_state.target,
        storage::stateIsSaved() ? ' ' : '*',
        static_cast<int>(power));

    if (!stateManager::configuration_correct) {
        std::memcpy(display[1], "   CONNECT PC   ", 16);
    } else if (stateManager::sleep) {
        std::memcpy(display[1], "     SLEEP      ", 16);
    } else if (stateManager::in_stand) {
        std::memcpy(display[1], "     STAND      ", 16);
    } else {
        int lastBarElement = static_cast<int>(16.0 * power / 100.0);
        for (int i = 0; i < 16; ++i) {
            display[1][i] = (i < lastBarElement) ? '#' : ' ';
        }
    }

    HAL::Display::write(display);

    char line1[17], line2[17];
    std::memcpy(line1, display[0], 16);
    line1[16] = '\0';
    std::memcpy(line2, display[1], 16);
    line2[16] = '\0';
    com::printf("DISP |%s%s|\n", line1, line2);
}

void setDisplaySettings(float backlightPercentage, float contrastPercentage) {
    core::settings.display.backlight = backlightPercentage;
    HAL::Display::setBacklight(backlightPercentage);

    core::settings.display.contrast = contrastPercentage;
    HAL::Display::setContrast(contrastPercentage);
}

}  // namespace display
}  // namespace core
