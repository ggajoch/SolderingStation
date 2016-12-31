#ifndef CORE_HAL_H_
#define CORE_HAL_H_

#include <cstdint>
#include "array_view.h"

namespace HAL {
void delay(std::uint32_t ms);
void wdgTick();

namespace Display {
void setBacklight(float percent);
void setContrast(float percent);
void write(char array[2][16]);
}

namespace Tip {
void setHeating(float percent);
uint16_t readRaw();
bool inStand();
}

namespace Com {
void puts(const char* data);
void setCallback(void callback(char* data));
}

namespace Encoder {
int getCountAndReset();
void setButtonCallback(void (*callback)());
void callbackTick();
}

namespace Memory {
void store(libs::array_view<const std::uint8_t> data);
void get(libs::array_view<std::uint8_t> data);
}

}  // namespace HAL

#endif  // CORE_HAL_H_
