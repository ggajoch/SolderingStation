#include "gtest/gtest.h"

#include "Tip.h"

constexpr float eps = 1e-5;

TEST(tempScaling, basic) {
    core::Tip tip;

    tip.offset = 0;
    tip.gain = 1;
    for (int i = 0; i < 1000; ++i) {
        EXPECT_NEAR(tip.getTemperature(i), i, eps);
    }

    tip.offset = 1;
    tip.gain = 2;
    for (int i = 0; i < 500; ++i) {
        EXPECT_NEAR(tip.getTemperature(i), 1+2.0*i, eps);
    }

    tip.offset = 20.5;
    tip.gain = 0.05;
    for (int i = 0; i < 500; ++i) {
        EXPECT_NEAR(tip.getTemperature(i), 20.5+0.05*i, eps);
    }
}
