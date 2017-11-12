#ifndef TARGETS_UNITTESTS_HALMOCK_H_
#define TARGETS_UNITTESTS_HALMOCK_H_

#include <queue>
#include "config.h"
#include "HAL.h"

namespace HAL {

namespace Display {
extern float backlight, contrast;
extern char line1[17], line2[17];
}  // namespace Display

namespace Tip {
extern float heatingPercentage;
extern std::queue<uint16_t> rawTemperatureData;
void set_temperature(uint16_t tmp);
extern bool in_stand;
}  // namespace Tip

namespace Com {
extern char lastLine[100];
bool checkLastLine(const char* line);
}  // namespace Com

namespace Encoder {
extern int count;
}  // namespace Encoder

namespace Memory {
extern std::array<uint8_t, static_cast<uint16_t>(core::config::memory_type)> table;
}  // namespace Memory

}  // namespace HAL

#endif  // TARGETS_UNITTESTS_HALMOCK_H_
