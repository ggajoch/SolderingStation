#include "timer.h"

namespace core::timer {

std::chrono::milliseconds actual{0};

void tick() {
    actual += config::tickDuration;
}

std::chrono::milliseconds now() {
    return actual;
}

}  // namespace core::timer
