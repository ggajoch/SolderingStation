#ifndef CORE_CORE_H_
#define CORE_CORE_H_

#include "Tip.h"
#include "pid.h"

namespace core {

void setup();
void tick();

extern Tip tip;
extern PID pid;
extern float temp, target, power;

};  // namespace core

#endif  // CORE_CORE_H_
