#include "timer.h"

namespace core {
namespace timer {

std::chrono::milliseconds actual{0};

void tick() {
    actual += config::tickDuration;
}

std::chrono::milliseconds now() {
    return actual;
}

}  // namespace timer
}  // namespace core
