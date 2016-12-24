#ifndef CORE_CORE_H_
#define CORE_CORE_H_

#include "pid.h"
#include "tempSensor.h"

namespace core {

void setup();
void tick();

extern PID pid;
extern float temp, target, power;

}  // namespace core

#endif  // CORE_CORE_H_
