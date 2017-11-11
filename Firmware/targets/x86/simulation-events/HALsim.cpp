#include <queue>
#include <cstdio>
#include <cstring>

#include "HAL.h"
#include "HALsim.h"

namespace HAL {

void delay(uint32_t ms) {}

namespace Display {
    void setBacklight(float percent) {}
    void setContrast(float percent) {}
    void write(char array[2][16]) {}
}  // namespace Display

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

    bool inStand() {
        return false;
    }
}  // namespace Tip

namespace Com {
    char lastLine[100];
    bool checkLastLine(const char * line) {
        bool res;
        if (std::strcmp(line, lastLine) == 0) {
            res = true;
        } else {
            res = false;
        }
        std::memset(lastLine, 0, sizeof(lastLine));
        return res;
    }

    void puts(const char * data) {
        std::printf("data: %s", data);
        std::strcpy(lastLine, data);
    }
    void (*callback)(char * data);
    void setCallback(void (*callback_)(char * data)) {
        callback = callback_;
    }
}  // namespace Com

namespace Encoder {
    int getCountAndReset() { return 0; }
    void setButtonCallback(void (*callback)()) {}
}  // namespace Encoder

namespace Memory {
void storeSettings(const core::storage::Settings& data) {}
void storeState(const core::storage::State& data) {}
std::experimental::optional<core::storage::Settings> getSettings() {}
std::experimental::optional<core::storage::State> getState() {}
}  // namespace Memory

}  // namespace HAL
