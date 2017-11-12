#ifndef CORE_ENCODER_H_
#define CORE_ENCODER_H_

#include "HAL.h"
#include "core.h"
#include "storage/persistent_state.h"

namespace core {
namespace encoder {

void tick() {
    auto encoder = HAL::Encoder::getCountAndReset();
    core::persistent_state.target += encoder * 5;
    if (core::persistent_state.target < 0)
        core::persistent_state.target = 0;
}

}  // namespace encoder
}  // namespace core

#endif  // CORE_ENCODER_H_
