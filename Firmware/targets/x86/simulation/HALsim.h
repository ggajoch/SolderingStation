#ifndef TARGETS_SIMULATION_HALSIM_H_
#define TARGETS_SIMULATION_HALSIM_H_

#include "HAL.h"
#include <queue>

namespace HAL {

namespace Display {
}  // namespace Display

namespace Tip {
    extern float heatingPercentage;
    extern bool inStandFlag;
    extern uint16_t temperature;
}  // namespace Tip

namespace Com {
    extern void (*callback)(char * data);
}  // namespace Com

namespace Encoder {
    extern int count;
    extern void (*callback)();
}  // namespace Encoder

namespace Memory {
}  // namespace Memory

}  // namespace HAL

#endif  // TARGETS_SIMULATION_HALSIM_H_
