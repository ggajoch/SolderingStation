#ifndef CORE_TIP_H_
#define CORE_TIP_H_

#include "HAL.h"

namespace core {
namespace tip {

struct Params {
    float offset, gain;
};


    bool inStand;

float getTemperature(int raw_reading) {
    return params.offset + params.gain*static_cast<float>(raw_reading);
}

    void tick() {
        inStand = HAL::Tip::inStand();
    }
};

};  // namespace core

#endif  // CORE_TIP_H_
