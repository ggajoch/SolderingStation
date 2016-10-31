#include <queue>
#include <cstdio>
#include <cstring>
#include "com.h"

#include "HAL.h"
#include "HALsim.h"

#include "Serial.h"

extern Serial * serial;

namespace HAL {

void delay(uint32_t ms) {}

namespace Display {
    void setBacklight(float percent);
    void setContrast(float percent);
    void write(char array[2][16])  {
        char line1[17], line2[17];
        std::memcpy(line1, array[0], 16);
        line1[16] = '\0';
        std::memcpy(line2, array[1], 16);
        line2[16] = '\0';
        core::com::printf("DISP |%s%s|\n", line1, line2);
    }
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

    bool inStandFlag = false;
    bool inStand() {
        return inStandFlag;
    }
};  // namespace Tip

namespace Com {
    void puts(const char * data) {
        std::printf("%s", data);
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
