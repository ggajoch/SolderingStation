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

void setBacklight(float percent) {
    backlight = percent;
}
void setContrast(float percent) {
    contrast = percent;
}
char line1[17], line2[17];
void write(char array[2][16]) {
    std::memcpy(line1, array[0], 16);
    line1[16] = '\0';
    std::memcpy(line2, array[1], 16);
    line2[16] = '\0';
}
}  // namespace Display

namespace Tip {
float heatingPercentage;
void setHeating(float percent) {
    printf("HEATING: %f\n", percent);
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
bool checkLastLine(const char* line) {
    bool res;
    if (std::strcmp(line, lastLine) == 0) {
        res = true;
    } else {
        res = false;
    }
    std::memset(lastLine, 0, sizeof(lastLine));
    return res;
}

void puts(const char* data) {
    std::printf("data: |%s|\n", data);
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
    printf("SAVE to %d: ", address);
    for(auto x: data) {
        printf("%d ", x);
    }
    printf("\n");
    ASSERT_LE(address + data.size(), table.size());
    std::copy(data.begin(), data.end(), table.begin()+address);
}
void get(uint16_t address, gsl::span<std::uint8_t> data) {
    printf("READ from %d: ", address);
    for(auto x: data) {
        printf("%d ", x);
    }
    printf("\n");
    ASSERT_LE(address + data.size(), table.size());
    std::copy(table.begin()+address, table.begin()+address+data.length(), data.begin());
}
}  // namespace Memory

}  // namespace HAL
