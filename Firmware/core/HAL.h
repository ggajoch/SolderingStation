#ifndef CORE_HAL_H_
#define CORE_HAL_H_

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
void handler(char* data); // implemented in core
}

namespace Encoder {
int getCountAndReset();
void buttonHandler(); // implemented in core
}

namespace Memory {
void set(uint16_t address, gsl::span<const std::uint8_t> data);
void get(uint16_t address, gsl::span<std::uint8_t> data);
}

}  // namespace HAL

#endif  // CORE_HAL_H_
