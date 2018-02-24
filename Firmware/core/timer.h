#pragma once

#include "config.h"

namespace core::timer {

void tick();
std::chrono::milliseconds now();

}  // namespace core::timer
