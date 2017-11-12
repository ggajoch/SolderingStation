#ifndef CORE_HAL_H_
#define CORE_HAL_H_

#include <cstdint>
#include <experimental/optional>
#include <gsl/span>
#include "storage.h"

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
//void setCallback(void callback(char* data));
}

namespace Encoder {
int getCountAndReset();
void setButtonCallback(void (*callback)());
void callbackTick();
//void button_handler(); // implemented in core
}

namespace Memory {
void storeSettings(const core::storage::Settings& data);
void storeState(const core::storage::State& data);
std::experimental::optional<core::storage::Settings> getSettings();
std::experimental::optional<core::storage::State> getState();
/*void store(gsl::span<const std::uint8_t> data);
void get(gsl::span<std::uint8_t> data);*/
}

}  // namespace HAL

#endif  // CORE_HAL_H_
