#include <cstdio>
#include <cstring>
#include <storage/storage.h>

#include "HAL.h"
#include "stateManager.h"
#include "com.h"

namespace core {
namespace display {

void tick() {
    char line1[17], line2[17];

    snprintf(line1, 17,
        "%3d/%3d %c   %3d%%",
        static_cast<int>(temp),
        core::persistent_state.target,
        storage::stateIsSaved() ? ' ' : '*',
        static_cast<int>(power));

    if (!stateManager::configuration_correct) {
        std::strncpy(line2, "   CONNECT PC   ", 17);
    } else if (stateManager::sleep) {
        std::strncpy(line2, "     SLEEP      ", 17);
    } else if (stateManager::in_stand) {
        std::strncpy(line2, "     STAND      ", 17);
    } else {
        int lastBarElement = static_cast<int>(16.0 * power / 100.0);
        for (int i = 0; i < 16; ++i) {
            line2[i] = (i < lastBarElement) ? '#' : ' ';
        }
        line2[16] = '\0';
    }

    HAL::Display::write(line1, line2);

    com::printf("DISP |%s%s|\n", line1, line2);
}

void setDisplaySettings(float backlightPercentage, float contrastPercentage) {
    core::settings.display.backlight = backlightPercentage;
    core::settings.display.contrast = contrastPercentage;

    HAL::Display::set(backlightPercentage, contrastPercentage);
}

}  // namespace display
}  // namespace core
