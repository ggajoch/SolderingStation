#include <stateManager.h>
#include "gtest/gtest.h"

#include "HALmock.h"
#include "core.h"
#include "display.h"

using namespace core::stateManager;

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


TEST(Display, sleep) {
    state = State::Sleep;

    core::tick();
    EXPECT_STREQ(HAL::Display::line2, "     sleep      ");
}

TEST(Display, in_stand) {
    state = State::InStand;
    HAL::Tip::in_stand = true;

    core::tick();
    EXPECT_STREQ(HAL::Display::line2, "    in stand    ");
}

TEST(Display, off) {
    state = State::Off;

    core::tick();
    EXPECT_STREQ(HAL::Display::line2, "      OFF       ");
}

TEST(Display, configuration_incorrect) {
    state = State::InvalidConfig;

    core::tick();
    EXPECT_STREQ(HAL::Display::line2, "   CONNECT PC   ");
}

TEST(Display, state_running) {
    state = State::On;

    core::settings.tipParams.offset = 0;
    core::settings.tipParams.gain = 1;

    core::power = 0;
    core::temp = 0;
    core::persistent_state.target = 0;
    core::display::tick();
    EXPECT_STREQ(HAL::Display::line1, "  0/  0       0%");
    EXPECT_STREQ(HAL::Display::line2, "                ");

    core::power = 6.249;
    core::temp = 999;
    core::persistent_state.target = 999;
    core::display::tick();
    EXPECT_STREQ(HAL::Display::line1, "999/999       6%");
    EXPECT_STREQ(HAL::Display::line2, "                ");

    core::power = 6.25;
    core::temp = 1;
    core::persistent_state.target = 150;
    core::display::tick();
    EXPECT_STREQ(HAL::Display::line1, "  1/150       6%");
    EXPECT_STREQ(HAL::Display::line2, "#               ");

    core::power = 6.99;
    core::temp = 87;
    core::persistent_state.target = 56;
    core::display::tick();
    EXPECT_STREQ(HAL::Display::line1, " 87/ 56       6%");
    EXPECT_STREQ(HAL::Display::line2, "#               ");

    core::power = 7;
    core::temp = 60;
    core::persistent_state.target = 101;
    core::display::tick();
    EXPECT_STREQ(HAL::Display::line1, " 60/101       7%");
    EXPECT_STREQ(HAL::Display::line2, "#               ");

    core::power = 12.499;
    core::display::tick();
    EXPECT_STREQ(HAL::Display::line1, " 60/101      12%");
    EXPECT_STREQ(HAL::Display::line2, "#               ");

    core::power = 12.5;
    core::display::tick();
    EXPECT_STREQ(HAL::Display::line1, " 60/101      12%");
    EXPECT_STREQ(HAL::Display::line2, "##              ");

    core::power = 18.749;
    core::display::tick();
    EXPECT_STREQ(HAL::Display::line1, " 60/101      18%");
    EXPECT_STREQ(HAL::Display::line2, "##              ");

    core::power = 18.75;
    core::display::tick();
    EXPECT_STREQ(HAL::Display::line1, " 60/101      18%");
    EXPECT_STREQ(HAL::Display::line2, "###             ");

    core::power = 24.99;
    core::display::tick();
    EXPECT_STREQ(HAL::Display::line1, " 60/101      24%");
    EXPECT_STREQ(HAL::Display::line2, "###             ");

    core::power = 25;
    core::display::tick();
    EXPECT_STREQ(HAL::Display::line1, " 60/101      25%");
    EXPECT_STREQ(HAL::Display::line2, "####            ");

    core::power = 99.99;
    core::display::tick();
    EXPECT_STREQ(HAL::Display::line1, " 60/101      99%");
    EXPECT_STREQ(HAL::Display::line2, "############### ");

    core::power = 100;
    core::display::tick();
    EXPECT_STREQ(HAL::Display::line1, " 60/101     100%");
    EXPECT_STREQ(HAL::Display::line2, "################");
}
