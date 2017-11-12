#include <queue>
#include <cstdio>
#include <cstring>
#include <thread>
#include "com.h"

#include "HAL.h"
#include "HALsim.h"

#include "storage.h"


namespace HAL {

void delay(uint32_t ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds{ms});
}

namespace Display {
    void setBacklight(float percent) {
        std::printf("display backlight %f\n", percent);
    }

    void setContrast(float percent) {
        std::printf("display contrast %f\n", percent);
    }

    void write(char /*array*/[2][16])  {
    }
}  // namespace Display

namespace Tip {
    float heatingPercentage;
    bool inStandFlag;
    uint16_t temperature;

    void setHeating(float percent) {
        std::printf("tip heating %f\n", percent);
    }

    uint16_t readRaw() {
        return temperature;
    }

    bool inStand() {
        return inStandFlag;
    }
}  // namespace Tip

namespace Com {
    void puts(const char * data) {
        std::printf("serial %s", data);
    }

    void (*callback)(char * data);
    void setCallback(void (*callback_)(char *)) {
        callback = callback_;
    }
}  // namespace Com

namespace Encoder {
    int getCountAndReset() {
        return 0;
    }

    void setButtonCallback(void (*)()) {
    }
    void callbackTick() {}
}  // namespace Encoder

namespace Memory {
void storeSettings(const core::storage::Settings&) {
    printf("mem save settings\n");
}
void storeState(const core::storage::State&) {
    printf("mem save state\n");
}
std::experimental::optional<core::storage::Settings> getSettings() {
    static constexpr core::storage::Settings elements = {
        .pidParams = {.Kp = 1.0, .Ki = 4.0, .Kd = 0},
        .tipParams = {.offset = 20, .gain = 0.11},
        .contrast = 27.5,
        .backlight = 100};

    return elements;
}
std::experimental::optional<core::storage::State> getState() {
    static constexpr core::storage::State elements = {.targetTemperature = 0};

    return elements;
}
}  // namespace Memory

}  // namespace HAL
