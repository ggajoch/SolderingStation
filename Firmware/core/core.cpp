#include "core.h"
#include "Controller.h"
#include "HAL.h"
#include "com.h"

namespace core {

void setup() {
    controller.setup();
    HAL::Com::setCallback(com::rxCommandCallback);
}

void tick() {
    controller.tick();
    com::dispatchCommand();
}

};  // namespace core
