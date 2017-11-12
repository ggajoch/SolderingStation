#include <algorithm>
#include "encoder.h"
#include "HAL.h"
#include "core.h"

namespace core {
namespace encoder {

void tick() {
    auto encoder = HAL::Encoder::getCountAndReset();
    int target = core::persistent_state.target + encoder * 5;

    core::persistent_state.target = static_cast<uint16_t>(std::max(0, target));
}

}  // namespace encoder
}  // namespace core
