#ifndef CORE_TIP_H_
#define CORE_TIP_H_

namespace core {
namespace tempSensor {

struct Params {
    float offset, gain;
};

extern Params params;

float getAveragedTemperature();

};  // namespace tempSensor
};  // namespace core

#endif  // CORE_TIP_H_
