#include <queue>
#include <cstdio>
#include <cstring>
#include <thread>
#include "com.h"

#include "HAL.h"
#include "HALsim.h"

#include "storage/persistent_state.h"
#include "settings.h"


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
}  // namespace Com

namespace Encoder {
    int getCountAndReset() {
        return 0;
    }
}  // namespace Encoder

namespace Memory {
std::array<uint8_t, 10000> table;
void set(uint16_t address, gsl::span<const std::uint8_t> data) {
    std::copy(data.begin(), data.end(), table.begin()+address);
}
void get(uint16_t address, gsl::span<std::uint8_t> data) {
    std::copy(table.begin()+address, table.begin()+address+data.length(), data.begin());
}
}  // namespace Memory

}  // namespace HAL
