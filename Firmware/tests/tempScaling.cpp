#include "gtest/gtest.h"

#include "Tip.h"

constexpr float eps = 1e-5;

TEST(tempScaling, basic) {
    core::Tip tip;

    tip.setParameters(0, 1);
    for (int i = 0; i < 1000; ++i) {
        EXPECT_NEAR(tip.getTemperature(i), i, eps);
    }

    tip.setParameters(1, 2);
    for (int i = 0; i < 500; ++i) {
        EXPECT_NEAR(tip.getTemperature(i), 1+2.0*i, eps);
    }

    tip.setParameters(20.5, 0.05);
    for (int i = 0; i < 500; ++i) {
        EXPECT_NEAR(tip.getTemperature(i), 20.5+0.05*i, eps);
    }
}
