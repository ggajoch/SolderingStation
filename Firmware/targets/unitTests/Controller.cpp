#include "gtest/gtest.h"

#include "config.h"
#include "Controller.h"
#include "HALmock.h"

constexpr float eps = 1e-5;

void testSetup() {
    while (!HAL::Tip::rawTemperatureData.empty())
        HAL::Tip::rawTemperatureData.pop();
}

TEST(Controller, setup) {
    using core::controller;
    testSetup();

    controller.setup();
    EXPECT_NEAR(HAL::Tip::heatingPercentage, 0, eps);
}

TEST(Controller, averaging) {
    using core::controller;
    testSetup();

    controller.setup();
    controller.tip.offset = 0;
    controller.tip.gain = 1;

    for (int i = 0; i < core::tempAverages; ++i) {
        HAL::Tip::rawTemperatureData.push(100);
    }
    controller.tick();

    EXPECT_NEAR(controller.temperatureAverage.get(), 100, eps);
}
