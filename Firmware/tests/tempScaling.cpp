#include "gtest/gtest.h"

#include "Tip.h"

constexpr float eps = 1e-5;

TEST(tempScaling, basic) {
    Tip::setParameters(0, 1);
    for(int i = 0; i < 1000; ++i) {
        EXPECT_NEAR(Tip::getTemperature(i), i, eps);
    }

    Tip::setParameters(1, 2);
    for(int i = 0; i < 500; ++i) {
        EXPECT_NEAR(Tip::getTemperature(i), 1+2.0*i, eps);
    }

    Tip::setParameters(20.5, 0.05);
    for(int i = 0; i < 500; ++i) {
        EXPECT_NEAR(Tip::getTemperature(i), 20.5+0.05*i, eps);
    }
}