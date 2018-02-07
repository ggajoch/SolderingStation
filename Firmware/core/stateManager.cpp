#include <cstdio>

#include "HAL.h"
#include "core.h"
#include "stateManager.h"

namespace core {
namespace stateManager {

bool sleep = true, in_stand;
bool configuration_correct = false;
uint8_t config_send_from_pc = 0;

void tick() {
    in_stand = HAL::Tip::inStand();
    if (config_send_from_pc == static_cast<uint8_t>(Command::Correct)) {
        configuration_correct = true;
    }

    uint16_t temp_limit = std::numeric_limits<uint16_t>::max();

    if (!configuration_correct) {
        temp_limit = 0;
    }

    if (in_stand) {
        temp_limit = core::settings.stand_temperature;
    }

    if (sleep) {
        temp_limit = core::settings.sleep_temperature;
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

void buttonHandler() {
    core::stateManager::sleep = !core::stateManager::sleep;
}

}  // namespace Encoder
}  // namespace HAL
