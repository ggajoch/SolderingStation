#include <queue>
#include <cstdio>
#include <cstring>

#include "HAL.h"
#include "HALsim.h"

#include "Serial.h"

extern Serial * serial;

namespace HAL {

void delay(uint32_t ms) {}

namespace Display {
    void setBacklight(float percent);
    void setContrast(float percent);
    void write(char array[2][16]);
};  // namespace Display

namespace Tip {
    float heatingPercentage;
    void setHeating(float percent) {
        heatingPercentage = percent;
    }

    uint16_t temperature;
    void setTemperature(float temp) {
        temperature = static_cast<uint16_t>(10*temp);
    }

    uint16_t readRaw() {
        return temperature;
    }

    bool inStand();
};  // namespace Tip

namespace Com {
    void puts(const char * data) {
        serial->WriteData(data, strlen(data));
    }

    void (*callback)(char * data);
    void setCallback(void (*callback_)(char * data)) {
        callback = callback_;
    }
};  // namespace Com

namespace Encoder {
    int getCount();
    void reset();
    void setButtonCallback(void (*callback)());
};  // namespace Encoder

namespace Memory {
    void store(size_t addr, libs::array_view<const uint8_t> data);
    void get(size_t addr, libs::array_view<uint8_t> data);
};  // namespace Memory

};  // namespace HAL
