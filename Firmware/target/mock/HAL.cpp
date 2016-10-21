#include "HAL.h"

namespace HAL {

bool timeTick();

namespace Display {
    void setBacklight(float percent);
    void setContrast(float percent);
    void write(char array[2][16]);
};  // namespace Display

namespace Tip {
    void setHeating(float percent) {
    }

    uint16_t readRaw() {
        return 0;
    }

    bool inStand();
};  // namespace Tip

namespace Com {
    void write(const char * data);
    void setCallback(void callback(const char * data));
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
