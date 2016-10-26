#ifndef CORE_HAL_H_
#define CORE_HAL_H_

#include <stdint.h>
#include "array_view.h"

namespace HAL {
bool timeTick();
void delay(uint32_t ms);

namespace Display {
void setBacklight(float percent);
void setContrast(float percent);
void write(char array[2][16]);
};

namespace Tip {
void setHeating(float percent);
uint16_t readRaw();
bool inStand();
};

namespace Com {
void puts(const char * data);
void setCallback(void callback(const char * data));
};

namespace Encoder {
int getCount();
void reset();
void setButtonCallback(void (*callback)());
};

namespace Memory {
void store(size_t addr, libs::array_view<const uint8_t> data);
void get(size_t addr, libs::array_view<uint8_t> data);
};

};  // namespace HAL

#endif  // CORE_HAL_H_
