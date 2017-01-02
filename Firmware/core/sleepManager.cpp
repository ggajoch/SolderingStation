#include <cstdio>

#include "HAL.h"
#include "core.h"
#include "sleepManager.h"

namespace core {
namespace sleepManager {

bool sleepState = true, standState;
bool configState = false;

void tick() {
    standState = HAL::Tip::inStand();
    if (standState || sleepState || !configState) {
        core::pid.target = 0;
    } else {
        core::pid.target = core::target;
    }
}

void encoderCallback() {
    std::printf("ENCODER\n");
    sleepState = !sleepState;
}

void configStateSet(bool now) {
    configState = now;
}

}  // namespace sleepManager
}  // namespace core
