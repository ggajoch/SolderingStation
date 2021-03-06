#include "gtest/gtest.h"

#include "HALmock.h"
#include "tempSensor.h"
#include "settings.h"

constexpr float eps = 1e-4;

TEST(tempScaling, basic) {
    core::settings.tip.offset = 0;
    core::settings.tip.gain = 1;

//    libs::Average<float, config::tempAverages> temperatureAverage;

#define CLEAR()                                                                                                                            \
    while (!HAL::Tip::rawTemperatureData.empty())                                                                                          \
    HAL::Tip::rawTemperatureData.pop()

    CLEAR();
    for (int i = 0; i < 1000; ++i) {
        HAL::Tip::rawTemperatureData.push(i);
        EXPECT_NEAR(core::tempSensor::getAveragedTemperature(), i, eps);
        CLEAR();
    }

    core::settings.tip.offset = 1;
    core::settings.tip.gain = 2;
    CLEAR();
    for (int i = 0; i < 1000; ++i) {
        HAL::Tip::rawTemperatureData.push(i);
        EXPECT_NEAR(core::tempSensor::getAveragedTemperature(), 1 + 2 * i, eps);
        CLEAR();
    }

    core::settings.tip.offset = 20.5;
    core::settings.tip.gain = 0.05;
    CLEAR();
    for (int i = 0; i < 1000; ++i) {
        HAL::Tip::rawTemperatureData.push(i);
        EXPECT_NEAR(core::tempSensor::getAveragedTemperature(), 20.5 + 0.05 * i, eps);
        CLEAR();
    }
}
