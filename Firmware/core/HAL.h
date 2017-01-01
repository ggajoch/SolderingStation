#ifndef CORE_HAL_H_
#define CORE_HAL_H_

#include <cstdint>
#include <experimental/optional>
#include "array_view.h"
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
void setCallback(void callback(char* data));
}

namespace Encoder {
int getCountAndReset();
void setButtonCallback(void (*callback)());
void callbackTick();
}

namespace Memory {
void storeSettings(const core::storage::Settings& data);
void storeState(const core::storage::State& data);
std::experimental::optional<core::storage::Settings> getSettings();
std::experimental::optional<core::storage::State> getState();
}

}  // namespace HAL

#endif  // CORE_HAL_H_
