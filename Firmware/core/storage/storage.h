#pragma once

#include <cstdint>
#include <cstring>
#include <gsl/span>
#include "core.h"
#include "display.h"
#include "pid.h"
#include "tempSensor.h"

namespace core::storage {

void read();

bool stateIsSaved();

// this function should be invoked on every tick
void tick();

}  // namespace core::storage
