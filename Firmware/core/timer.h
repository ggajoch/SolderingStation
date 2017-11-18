#pragma once

#include "config.h"

namespace core {
namespace timer {

void tick();
std::chrono::milliseconds now();

}  // namespace timer
}  // namespace core
