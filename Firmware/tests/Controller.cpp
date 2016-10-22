#include "gtest/gtest.h"

#include "config.h"
#include "Controller.h"
#include "HALmock.h"

constexpr float eps = 1e-5;

TEST(Controller, setup) {
    using core::controller;

    controller.setup();
    EXPECT_NEAR(HAL::Tip::heatingPercentage, 0, eps);
}

TEST(Controller, averaging) {
    using core::controller;

    controller.setup();
    controller.tip.setParameters(0, 1);

    for (int i = 0; i < core::tempAverages; ++i) {
        HAL::Tip::rawTemperatureData.push(100);
    }
    controller.loop();

    EXPECT_NEAR(controller.temperatureAverage.get(), 100, eps);
}