#include <stateManager.h>
#include "gtest/gtest.h"

#include "HALmock.h"
#include "core.h"
#include "display.h"

TEST(Display, set_settings) {
    auto test = [](float backlight, float contrast) {
        core::settings.display.backlight = 0;
        core::settings.display.contrast = 1;
        HAL::Display::backlight = 2;
        HAL::Display::contrast = 3;

        core::display::setDisplaySettings(backlight, contrast);

        EXPECT_FLOAT_EQ(core::settings.display.backlight, backlight);
        EXPECT_FLOAT_EQ(core::settings.display.contrast, contrast);
        EXPECT_FLOAT_EQ(HAL::Display::backlight, backlight);
        EXPECT_FLOAT_EQ(HAL::Display::contrast, contrast);
    };
    test(10.0, 11.1);
    test(70.0, 98.45);
    test(55.0, 8596.23);
}

TEST(Display, display_cmd) {
    auto test = [](float backlight, float contrast) {
        core::settings.display.backlight = -17.1f;
        core::settings.display.contrast = 20.5f;
        HAL::Display::backlight = 5;
        HAL::Display::contrast = 6;

        char cmd[100];
        sprintf(cmd, "disp %f %f", backlight, contrast);
        HAL::Com::handler(cmd);

        EXPECT_FLOAT_EQ(core::settings.display.backlight, -17.1);
        EXPECT_FLOAT_EQ(core::settings.display.contrast, 20.5);
        EXPECT_FLOAT_EQ(HAL::Display::backlight, 5);
        EXPECT_FLOAT_EQ(HAL::Display::contrast, 6);

        core::tick();

        EXPECT_FLOAT_EQ(core::settings.display.backlight, backlight);
        EXPECT_FLOAT_EQ(core::settings.display.contrast, contrast);
        EXPECT_FLOAT_EQ(HAL::Display::backlight, backlight);
        EXPECT_FLOAT_EQ(HAL::Display::contrast, contrast);
    };
    test(10.0f, 11.5f);
    test(70.0f, -25.6f);
    test(55.0f, 59.23f);
}
