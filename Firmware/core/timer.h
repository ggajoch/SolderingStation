#ifndef CORE_TIMER_H_
#define CORE_TIMER_H_

#include "config.h"

namespace core {
namespace timer {

void tick();
std::chrono::milliseconds now();

};  // namespace timer
};  // namespace core

#endif  // CORE_TIMER_H_
