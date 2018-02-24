#pragma once

#include <queue>
#include "config.h"
#include "HAL.h"

namespace HAL {

namespace Display {
extern uint8_t backlight, contrast;
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
void checkLastLine(const char* line);
}  // namespace Com

namespace Encoder {
extern int count;
}  // namespace Encoder

namespace Memory {
extern std::array<uint8_t, static_cast<uint16_t>(core::config::memory_type)> table;
}  // namespace Memory

}  // namespace HAL
