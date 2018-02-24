#include "encoder.h"
#include <algorithm>
#include "HAL.h"
#include "core.h"

namespace core::encoder {

void tick() {
    auto encoder = HAL::Encoder::getCountAndReset();
    int target = core::persistent_state.target + encoder * 5;

    if (target < 0) {
        target = 0;
    }

    if (target > settings.tip.max_safe_temperature) {
        target = settings.tip.max_safe_temperature;
    }

    core::persistent_state.target = target;
}

}  // namespace core::encoder
