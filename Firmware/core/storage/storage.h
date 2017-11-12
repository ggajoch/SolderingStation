#ifndef CORE_STORAGE_H_
#define CORE_STORAGE_H_

#include <cstdint>
#include <cstring>
#include <gsl/span>
#include "core.h"
#include "pid.h"
#include "tempSensor.h"
#include "display.h"

namespace core {
namespace storage {

void read();

bool stateIsSaved();

// this function should be invoked on every tick
void tick();

}  // namespace storage
}  // namespace core

#endif  // CORE_STORAGE_H_
