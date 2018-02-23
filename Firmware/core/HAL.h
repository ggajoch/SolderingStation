#pragma once

#include <gsl/span>

namespace HAL {
void delay(std::uint32_t ms);

namespace Display {
void set(float backlight_percent, float contrast_percent);
void write(char line1[17], char line2[17]);
}  // namespace Display

namespace Tip {
void setHeating(float percent);
uint16_t readRaw();
bool inStand();
}  // namespace Tip

namespace Com {
void puts(const char* data);
void handler(char* data);  // implemented in core
}  // namespace Com

namespace Encoder {
int getCountAndReset();
void buttonPressedHandler();   // implemented in core
void buttonReleasedHandler();  // implemented in core
}  // namespace Encoder

namespace Memory {
void set(uint16_t address, gsl::span<const std::uint8_t> data);
void get(uint16_t address, gsl::span<std::uint8_t> data);
}  // namespace Memory

}  // namespace HAL
