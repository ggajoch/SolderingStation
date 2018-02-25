#include "encoder.h"
#include <algorithm>
#include "HAL.h"
#include "core.h"

namespace core::encoder {

void tick() {
    auto encoder = HAL::Encoder::getCountAndReset();
    int target = core::persistent_state.target + encoder * 5;

    core::persistent_state.target = std::clamp(target, 0, (int)settings.tip.max_safe_temperature);
}

}  // namespace core::encoder
