#include <queue>
#include <cstdio>
#include <cstring>
#include <thread>
#include <cassert>
#include <config.h>
#include <ncurses.h>
#include "com.h"

#include "HAL.h"
#include "HALsim.h"

#include "storage/persistent_state.h"
#include "settings.h"
#include "plantModel.h"

extern WINDOW *window;
extern TipModel model;

namespace HAL {

void delay(uint32_t ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds{ms});
}

namespace Display {
    void set(float backlight_percent, float contrast_percent) {
        mvprintw(3, 0, "backlight: %.2f%%   ", backlight_percent);
        mvprintw(3, 20, "contrast: %.2f%%   ", contrast_percent);
        refresh();
    }

    char line1[17], line2[17];
    void write(char line1_[17], char line2_[17]) {
        std::strcpy(line1, line1_);
        std::strcpy(line2, line2_);

        mvprintw(0, 0, "%s", line1);
        mvprintw(1, 0, "%s", line2);
        refresh();
    }
}  // namespace Display

namespace Tip {
    float heatingPercentage;
    bool inStandFlag = true;

    void setHeating(float percent) {
        heatingPercentage = percent;
        mvprintw(4, 20, "heating: %.2f%%    ", percent);
        refresh();
    }

    uint16_t readRaw() {
        return 10*model.Ttip;
    }

    bool inStand() {
        return inStandFlag;
    }
}  // namespace Tip

namespace Com {
    void puts(const char * data) {
//        wprintw(window, "serial %s", data);
//        wrefresh(window);
    }
}  // namespace Com

namespace Encoder {
    int count;
    int getCountAndReset() {
        auto now = count;
        count = 0;
        return now;
    }
}  // namespace Encoder

namespace Memory {
std::array<uint8_t, static_cast<uint16_t>(core::config::memory_type)> table;
void set(uint16_t address, gsl::span<const std::uint8_t> data) {
//    printf("SAVE to %d: ", address);
//    for(auto x: data) {
//        printf("%d ", x);
//    }
//    printf("\n");
    assert(address + data.size() <= table.size());
    std::copy(data.begin(), data.end(), table.begin()+address);
}
void get(uint16_t address, gsl::span<std::uint8_t> data) {
//    printf("READ from %d: ", address);
    assert(address + data.size() <= table.size());
    std::copy(table.begin()+address, table.begin()+address+data.length(), data.begin());
//    for(auto x: data) {
//        printf("%d ", x);
//    }
//    printf("\n");
}
}  // namespace Memory

}  // namespace HAL
