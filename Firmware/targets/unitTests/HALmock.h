#ifndef TARGETS_UNITTESTS_HALMOCK_H_
#define TARGETS_UNITTESTS_HALMOCK_H_

#include "HAL.h"
#include <queue>

namespace HAL {

namespace Display {
	extern float backlight, contrast;
}  // namespace Display

namespace Tip {
    extern float heatingPercentage;
    extern std::queue<uint16_t> rawTemperatureData;
}  // namespace Tip

namespace Com {
    extern char lastLine[100];
    bool checkLastLine(const char * line);
}  // namespace Com

namespace Encoder {
}  // namespace Encoder

namespace Memory {
}  // namespace Memory

}  // namespace HAL

#endif  // TARGETS_UNITTESTS_HALMOCK_H_
