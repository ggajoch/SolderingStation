#include <cstdio>

#include "HAL.h"
#include "storage/persistent_state.h"
#include "stateManager.h"
#include "core.h"

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
    if (in_stand || sleep || !configuration_correct) {
        core::pid.target = 0;
    } else {
        // 0.5 to follow temperature between full degrees
        core::pid.target = core::persistent_state.target - 0.5f;
    }
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
