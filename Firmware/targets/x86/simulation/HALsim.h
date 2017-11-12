#ifndef TARGETS_SIMULATION_HALSIM_H_
#define TARGETS_SIMULATION_HALSIM_H_

#include "HAL.h"
#include <queue>

namespace HAL {

namespace Tip {
    extern float heatingPercentage;
    extern bool inStandFlag;
    extern uint16_t temperature;
}  // namespace Tip

namespace Encoder {
    extern int count;
}  // namespace Encoder

}  // namespace HAL

#endif  // TARGETS_SIMULATION_HALSIM_H_
