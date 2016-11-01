#ifndef SOLDERINGSTATION_ENCODER_H
#define SOLDERINGSTATION_ENCODER_H

#include "HAL.h"
#include "core.h"

namespace core {
namespace encoder {

void tick() {
    auto encoder = HAL::Encoder::getCountAndReset();
    core::target += encoder * 5;
}

};  // namespace encoder
};  // namespace core

#endif //SOLDERINGSTATION_ENCODER_H
