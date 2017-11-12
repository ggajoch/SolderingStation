#include <cstdio>

#include "HAL.h"
#include "storage/persistent_state.h"
#include "stateManager.h"
#include "core.h"

namespace core {
namespace stateManager {

bool sleep = true, in_stand;
bool configuration_correct = false;

void tick() {
    in_stand = HAL::Tip::inStand();
    if (in_stand || sleep || !configuration_correct) {
        core::pid.target = 0;
    } else {
        // 0.5 to follow temperature between full degrees
        core::pid.target = core::persistent_state.target - 0.5f;
    }
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
