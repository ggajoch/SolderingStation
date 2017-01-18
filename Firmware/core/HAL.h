#ifndef CORE_HAL_H_
#define CORE_HAL_H_

#include <cstdint>
#include <gsl/span>

namespace HAL {
void delay(std::uint32_t ms);

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
void store(gsl::span<const std::uint8_t> data);
void get(gsl::span<std::uint8_t> data);
}

}  // namespace HAL

#endif  // CORE_HAL_H_
