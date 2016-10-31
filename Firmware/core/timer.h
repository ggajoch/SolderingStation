#ifndef SOLDERINGSTATION_TIMER_H
#define SOLDERINGSTATION_TIMER_H

#include "config.h"

namespace core {
namespace timer {

void tick();
std::chrono::milliseconds now();

};  // namespace core
};  // namespace timer

#endif //SOLDERINGSTATION_TIMER_H
