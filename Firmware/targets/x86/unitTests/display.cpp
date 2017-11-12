#include <stateManager.h>
#include "gtest/gtest.h"

#include "HALmock.h"
#include "core.h"
#include "display.h"

TEST(Display, backlight_raw) {
    auto test = [](float val) {
        core::display::backlight = 0;
        core::display::contrast = 78.5;
        HAL::Display::backlight = 1;
        HAL::Display::contrast = 2;

        core::display::setBacklight(val);

        EXPECT_FLOAT_EQ(core::display::backlight, val);
        EXPECT_FLOAT_EQ(core::display::contrast, 78.5);
        EXPECT_FLOAT_EQ(HAL::Display::backlight, val);
        EXPECT_FLOAT_EQ(HAL::Display::contrast, 2);
    };
    test(10.0);
    test(70.0);
    test(55.0);
}

TEST(Display, contrast_raw) {
    auto test = [](float val) {
        core::display::backlight = 15.0;
        core::display::contrast = 7.1;
        HAL::Display::backlight = 3;
        HAL::Display::contrast = 4;

        core::display::setContrast(val);

        EXPECT_FLOAT_EQ(core::display::backlight, 15.0);
        EXPECT_FLOAT_EQ(core::display::contrast, val);
        EXPECT_FLOAT_EQ(HAL::Display::backlight, 3);
        EXPECT_FLOAT_EQ(HAL::Display::contrast, val);
    };
    test(10.0);
    test(70.0);
    test(55.0);
}

TEST(Display, contrast_cmd) {
    auto test = [](float backlight, float contrast) {
        core::display::backlight = -17.1f;
        core::display::contrast = 20.5f;
        HAL::Display::backlight = 5;
        HAL::Display::contrast = 6;

        char cmd[100];
        sprintf(cmd, "disp %f %f", backlight, contrast);
        HAL::Com::handler(cmd);

        EXPECT_FLOAT_EQ(core::display::backlight, -17.1);
        EXPECT_FLOAT_EQ(core::display::contrast, 20.5);
        EXPECT_FLOAT_EQ(HAL::Display::backlight, 5);
        EXPECT_FLOAT_EQ(HAL::Display::contrast, 6);

        core::tick();

        EXPECT_FLOAT_EQ(core::display::backlight, backlight);
        EXPECT_FLOAT_EQ(core::display::contrast, contrast);
        EXPECT_FLOAT_EQ(HAL::Display::backlight, backlight);
        EXPECT_FLOAT_EQ(HAL::Display::contrast, contrast);
    };
    test(10.0f, 11.5f);
    test(70.0f, -25.6f);
    test(55.0f, 59.23f);
}
