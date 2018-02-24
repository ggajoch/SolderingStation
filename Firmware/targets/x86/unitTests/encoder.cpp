#include <core.h>
#include "gtest/gtest.h"

#include "HALmock.h"
#include "storage/persistent_state.h"

TEST(Encoder, rotation) {
    core::settings.tip.max_safe_temperature = 65000;

    auto test = [](int ticks) {
        auto temp_now = core::persistent_state.target;

        HAL::Encoder::count = ticks;

        core::tick();

        EXPECT_EQ(HAL::Encoder::count, 0);

        EXPECT_FLOAT_EQ(core::persistent_state.target, temp_now + 5 * ticks);
    };
    core::persistent_state.target = 5;

    test(10);
    test(55);
    test(-65);
    test(70);
    test(-30);
    test(10);
    test(-50);

    test(-1);
    EXPECT_EQ(core::persistent_state.target, 0);
}

TEST(Encoder, temperatureCannotBeLowerThanZero) {
    auto test = [](int ticks, int expected) {
        HAL::Encoder::count = ticks;
        core::tick();
        EXPECT_EQ(HAL::Encoder::count, 0);
        EXPECT_FLOAT_EQ(core::persistent_state.target, expected);
    };
    core::persistent_state.target = 5;

    test(-1, 0);
    test(-10, 0);
    test(10, 50);
    test(-9, 5);
    test(10, 55);

    test(-50, 0);
    test(1, 5);
    test(-10, 0);
}