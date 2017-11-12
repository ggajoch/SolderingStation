#include "gtest/gtest.h"

#include "HALmock.h"
#include "config.h"
#include "core.h"

constexpr float eps = 1e-5;

void testSetup() {
    while (!HAL::Tip::rawTemperatureData.empty())
        HAL::Tip::rawTemperatureData.pop();
}

TEST(Controller, setup) {
    testSetup();

    core::setup();
    EXPECT_NEAR(HAL::Tip::heatingPercentage, 0, eps);
}

TEST(Controller, averaging) {
    testSetup();

    core::setup();
    core::settings.tipParams.offset = 0;
    core::settings.tipParams.gain = 1;

    for (int i = 0; i < core::config::tempAverages; ++i) {
        HAL::Tip::rawTemperatureData.push(100);
    }
    core::tick();

    //    EXPECT_NEAR(core::temperatureAverage.get(), 100, eps);
}
