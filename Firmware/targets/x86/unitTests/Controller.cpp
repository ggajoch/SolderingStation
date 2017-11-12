#include "gtest/gtest.h"

#include "HALmock.h"
#include "config.h"
#include "core.h"

constexpr float eps = 1e-5;

TEST(Controller, setup) {
    HAL::Tip::heatingPercentage = 10;
    core::setup();
    EXPECT_FLOAT_EQ(HAL::Tip::heatingPercentage, 0);
}

void clear() {
    while (!HAL::Tip::rawTemperatureData.empty())
        HAL::Tip::rawTemperatureData.pop();
}

TEST(Controller, averaging) {
    clear();

    core::settings.tipParams.offset = 0;
    core::settings.tipParams.gain = 1;

    for (int i = 0; i < core::config::tempAverages; ++i) {
        HAL::Tip::rawTemperatureData.push(i % 2);
    }
    core::tick();

    EXPECT_EQ(HAL::Tip::rawTemperatureData.size(), 1); // leftover for testing
    EXPECT_FLOAT_EQ(core::temp, 0.5);

    clear();

    HAL::Tip::rawTemperatureData.push(50);
    for (int i = 1; i < core::config::tempAverages; ++i) {
        HAL::Tip::rawTemperatureData.push(i);
        printf("%d ", i);
    }
    EXPECT_EQ(HAL::Tip::rawTemperatureData.size(), 100);

    core::tick();

    EXPECT_EQ(HAL::Tip::rawTemperatureData.size(), 1); // leftover for testing
    EXPECT_FLOAT_EQ(core::temp, 50.0);
}
