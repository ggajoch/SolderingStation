#ifndef CORE_ENCODER_H_
#define CORE_ENCODER_H_

#include "HAL.h"
#include "core.h"

namespace core {
namespace encoder {

void tick() {
    auto encoder = HAL::Encoder::getCountAndReset();
    core::target += encoder * 5;
    if (core::target < 0)
        core::target = 0;
}

}  // namespace encoder
}  // namespace core

#endif  // CORE_ENCODER_H_
