#include <cstdio>
#include <cstring>

#include "HAL.h"
#include "core.h"
#include "sleepManager.h"
#include "com.h"
#include "storage/persistent_state.h"

namespace core {
namespace display {

void tick() {
    char display[2][16];

    std::sprintf(display[0],
        "%3d/%3d %c   %3d%%",
        static_cast<int>(temp),
        static_cast<int>(core::persistent_state.target),
        storage::stateIsSaved() ? ' ' : '*',
        static_cast<int>(power));

    if (!sleepManager::configurationCorrectState) {
        std::memcpy(display[1], "   CONNECT PC   ", 16);
    } else if (sleepManager::sleepState) {
        std::memcpy(display[1], "     SLEEP      ", 16);
    } else if (sleepManager::standState) {
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

float backlight, contrast;

void setBacklight(float percentage) {
    backlight = percentage;
    HAL::Display::setBacklight(backlight);
}

void setContrast(float percentage) {
    contrast = percentage;
    HAL::Display::setContrast(contrast);
}

}  // namespace display
}  // namespace core
