#include <cstdio>
#include <cstring>
#include <queue>
#include <gtest/gtest.h>

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
void write(char array[2][16]) {
}
}  // namespace Display

namespace Tip {
float heatingPercentage;
void setHeating(float percent) {
    printf("HEATING: %f\n", percent);
    heatingPercentage = percent;
}

std::queue<uint16_t> rawTemperatureData;
uint16_t readRaw() {
    uint16_t now = rawTemperatureData.front();
    if (rawTemperatureData.size() > 1) {
        rawTemperatureData.pop();
    }
    return now;
}

bool inStand() {
    return false;
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

void (*callback)(char* data);
void setCallback(void (*callback_)(char* data)) {
    callback = callback_;
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
    ASSERT_LT(address + data.size(), table.size());
    std::copy(data.begin(), data.end(), table.begin()+address);
}
void get(uint16_t address, gsl::span<std::uint8_t> data) {
    ASSERT_LT(address + data.size(), table.size());
    std::copy(table.begin()+address, table.begin()+address+data.length(), data.begin());
}
}  // namespace Memory

}  // namespace HAL
