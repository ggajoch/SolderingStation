#ifndef CORE_HAL_H_
#define CORE_HAL_H_

#include <cstdint>
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
void storeSettings(core::storage::Settings* data);
void getSettings(core::storage::Settings* data);
void storeState(core::storage::State* data);
void getState(core::storage::State* data);
}

}  // namespace HAL

#endif  // CORE_HAL_H_
