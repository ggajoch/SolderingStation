#pragma once

#include "pid.h"
#include "storage/persistent_state.h"

namespace core {

void setup();
void tick();

extern PID pid;
extern float temp, power;

extern PersistentState persistent_state;

}  // namespace core
