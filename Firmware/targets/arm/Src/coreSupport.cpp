#include "core.h"

extern "C" void CORE_setup() {
	core::setup();
}

extern "C" void CORE_tick() {
	core::tick();
}
