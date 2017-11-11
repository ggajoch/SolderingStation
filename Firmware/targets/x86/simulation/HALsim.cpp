#include <queue>
#include <cstdio>
#include <cstring>
#include "com.h"

#include "HAL.h"
#include "HALsim.h"

#include "Serial.h"
#include "storage.h"

extern Serial * serial;

namespace HAL {

void delay(uint32_t ms) {}

namespace Display {
    void setBacklight(float percent) {
        std::printf("Setting backlight: %f\n", percent);
    }

    void setContrast(float percent) {
        std::printf("Setting contrast: %f\n", percent);
    }

    void write(char array[2][16])  {
        char line1[17], line2[17];
        std::memcpy(line1, array[0], 16);
        line1[16] = '\0';
        std::memcpy(line2, array[1], 16);
        line2[16] = '\0';
        core::com::printf("DISP |%s%s|\n", line1, line2);
    }
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

    bool inStandFlag = false;
    bool inStand() {
        return inStandFlag;
    }
}  // namespace Tip

namespace Com {
    void puts(const char * data) {
//        std::printf("%s", data);
        serial->WriteData(data, strlen(data));
    }

    void (*callback)(char * data);
    void setCallback(void (*callback_)(char * data)) {
        callback = callback_;
    }
}  // namespace Com

namespace Encoder {
    int count;
    int getCountAndReset() {
        int now = count;
        count = 0;
        return now;
    }
    void reset();

    void (*callback)();
    void setButtonCallback(void (*callback_)()) {
        callback = callback_;
    }
}  // namespace Encoder

namespace Memory {
void storeSettings(const core::storage::Settings&) {
    Com::puts("SAVING TO MEMORY Settings\n");
}
void storeState(const core::storage::State&) {
    Com::puts("SAVING TO MEMORY State\n");
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
