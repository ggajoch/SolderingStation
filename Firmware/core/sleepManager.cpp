#include <cstdio>

#include "HAL.h"
#include "core.h"
#include "sleepManager.h"

namespace core {
namespace sleepManager {

bool sleepState = true, standState;

void tick() {
    standState = HAL::Tip::inStand();
    if (standState || sleepState) {
        core::pid.target = 0;
    } else {
        core::pid.target = core::target;
    }
}

void encoderCallback() {
    std::printf("ENCODER\n");
    sleepState = !sleepState;
}

}  // namespace sleepManager
}  // namespace core
