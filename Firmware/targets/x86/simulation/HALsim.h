#ifndef TARGETS_SIMULATION_HALSIM_H_
#define TARGETS_SIMULATION_HALSIM_H_

#include "HAL.h"
#include <queue>

namespace HAL {

namespace Display {
}  // namespace Display

namespace Tip {
    extern float heatingPercentage;
    void setTemperature(float temp);
    extern bool inStandFlag;
}  // namespace Tip

namespace Com {
    extern char lastLine[100];
    bool checkLastLine(const char * line);
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
