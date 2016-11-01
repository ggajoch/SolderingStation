#ifndef CORE_TEMPSENSOR_H_
#define CORE_TEMPSENSOR_H_

namespace core {
namespace tempSensor {

struct Params {
    float offset, gain;
};

extern Params params;

float getAveragedTemperature();

};  // namespace tempSensor
};  // namespace core

#endif  // CORE_TEMPSENSOR_H_
