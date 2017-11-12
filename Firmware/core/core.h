#ifndef CORE_CORE_H_
#define CORE_CORE_H_

#include "pid.h"
#include "storage/persistent_state.h"

namespace core {

void setup();
void tick();

extern PID pid;
extern float temp, power;

extern PersistentState persistent_state;

}  // namespace core

#endif  // CORE_CORE_H_
