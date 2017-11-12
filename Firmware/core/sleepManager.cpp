#include <cstdio>

#include "HAL.h"
#include "storage/persistent_state.h"
#include "sleepManager.h"

namespace core {
namespace sleepManager {

bool sleepState = true, standState;
bool configurationCorrectState = false;

void tick() {
    standState = HAL::Tip::inStand();
    if (standState || sleepState || !configurationCorrectState) {
        core::pid.target = 0;
    } else {
        core::pid.target = core::persistent_state.target;
    }
}

}  // namespace sleepManager
}  // namespace core

namespace HAL {
namespace Encoder {

void buttonHandler() {
    core::sleepManager::sleepState = !core::sleepManager::sleepState;
}

}  // namespace Encoder
}  // namespace HAL
