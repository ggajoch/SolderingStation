#include <cstdio>
#include <cstring>
#include <queue>
#include <gtest/gtest.h>
#include <config.h>

#include "HAL.h"
#include "HALmock.h"

namespace HAL {

void delay(uint32_t ms) {
}

namespace Display {
float backlight, contrast;
void set(float backlight_percent, float contrast_percent) {
    backlight = backlight_percent;
    contrast = contrast_percent;
}

char line1[17], line2[17];
void write(char line1_[17], char line2_[17]) {
    std::strcpy(line1, line1_);
    std::strcpy(line2, line2_);
}
}  // namespace Display

namespace Tip {
float heatingPercentage;
void setHeating(float percent) {
    heatingPercentage = percent;
}

std::queue<uint16_t> rawTemperatureData;
void set_temperature(uint16_t tmp) {
    while (!rawTemperatureData.empty()) {
        rawTemperatureData.pop();
    }
    rawTemperatureData.push(tmp);
}

uint16_t readRaw() {
    uint16_t now = rawTemperatureData.front();
    if (rawTemperatureData.size() > 1) {
        rawTemperatureData.pop();
    }
    return now;
}

bool in_stand;
bool inStand() {
    return in_stand;
}
}  // namespace Tip

namespace Com {
char lastLine[100];
void checkLastLine(const char* line) {
    EXPECT_STREQ(line, lastLine);
    std::memset(lastLine, 0, sizeof(lastLine));
}

void puts(const char* data) {
    std::strcpy(lastLine, data);
}
}  // namespace Com

namespace Encoder {
int count;
int getCountAndReset() {
    auto tmp = count;
    count = 0;
    return tmp;
}
}  // namespace Encoder

namespace Memory {
std::array<uint8_t, static_cast<uint16_t>(core::config::memory_type)> table;
void set(uint16_t address, gsl::span<const std::uint8_t> data) {
    ASSERT_LE(address + data.size(), table.size());
    std::copy(data.begin(), data.end(), table.begin()+address);
}
void get(uint16_t address, gsl::span<std::uint8_t> data) {
    ASSERT_LE(address + data.size(), table.size());
    std::copy(table.begin()+address, table.begin()+address+data.size(), data.begin());
}
}  // namespace Memory

}  // namespace HAL
