#include <cstdio>
#include <chrono>
#include <experimental/optional>

#include "HAL.h"
#include "core.h"
#include "stateManager.h"
#include "timer.h"

using namespace std::chrono_literals;

namespace core {
namespace stateManager {

bool off = true, sleep = false, in_stand;
bool configuration_correct = false;
uint8_t config_send_from_pc = 0;

bool button_pressed, button_released;
std::experimental::optional<std::chrono::milliseconds> time_when_pressed;

template<typename T>
void clamp(uint16_t& temp_limit, T limit) {
    if (limit < temp_limit) {
        temp_limit = limit;
    }
}

void tick() {
    static auto old_in_stand = false;
    in_stand = HAL::Tip::inStand();
    if (in_stand != old_in_stand) {
        core::pid.reset();
    }
    old_in_stand = in_stand;

    if (config_send_from_pc == static_cast<uint8_t>(Command::Correct)) {
        configuration_correct = true;
    }

    if (button_pressed) {
        button_pressed = false;
        core::stateManager::time_when_pressed = core::timer::now();
    }

    // button held for more than 1s
    if (time_when_pressed && core::timer::now() - *time_when_pressed > 1s) {
        time_when_pressed = {};
        core::stateManager::off = !core::stateManager::off;
        core::pid.reset();
    }

    // button released - handle sleep event
    if (button_released) {
        button_released = false;
        if (time_when_pressed) {
            core::stateManager::sleep = !core::stateManager::sleep;
            core::stateManager::time_when_pressed = {};
            core::pid.reset();
        }
    }

    uint16_t temp_limit = std::numeric_limits<uint16_t>::max();

    if (!configuration_correct) {
        clamp(temp_limit, 0);
    }

    if (off) {
        clamp(temp_limit, 0);
        sleep = false;
    }

    if (in_stand) {
        clamp(temp_limit, core::settings.stand_temperature);
    }

    if (sleep) {
        clamp(temp_limit, core::settings.sleep_temperature);
    }

    // 0.5 to follow temperature between full degrees
    core::pid.target = std::min(core::persistent_state.target, temp_limit) - 0.5f;
}

void config_command_received(Command cmd) {
    config_send_from_pc |= static_cast<uint8_t>(cmd);
}

}  // namespace stateManager
}  // namespace core

namespace HAL {
namespace Encoder {

void buttonPressedHandler() {
    core::stateManager::button_pressed = true;
}

void buttonReleasedHandler() {
    core::stateManager::button_released = true;
}

}  // namespace Encoder
}  // namespace HAL
