#include <stateManager.h>
#include "gtest/gtest.h"

#include "HALmock.h"
#include "core.h"
#include "display.h"

class StateManager : public ::testing::Test {
    void SetUp() {
        core::stateManager::configuration_correct = true;
        core::stateManager::sleep = false;
        HAL::Tip::in_stand = false;
        core::settings.pidParams.Kp = 100;
        core::persistent_state.target = 101;

        HAL::Tip::set_temperature(100);
        core::settings.tipParams.offset = 10;
        core::settings.tipParams.gain = 0.5;

        check_running();
    }

 public:
    void check_idle() {
        for(int i = 0; i < 10; ++i) {
            EXPECT_STREQ(HAL::Display::line1, " 60/101 *     0%");
            core::tick();
            EXPECT_FLOAT_EQ(core::pid.target, 0);
            EXPECT_FLOAT_EQ(core::power, 0);
            EXPECT_FLOAT_EQ(HAL::Tip::heatingPercentage, 0);
        }
    }

    void check_running() {
        for(int i = 0; i < 10; ++i) {
            core::tick();

            char tmp[17];
            sprintf(tmp, " 60/101 *   %3d%%", (int)core::power);
            EXPECT_STREQ(HAL::Display::line1, tmp);

            EXPECT_GT(core::power, 0);
            EXPECT_GT(HAL::Tip::heatingPercentage, 0);
            EXPECT_FLOAT_EQ(core::pid.target, 101.0 - 0.5);
        }
    }
};

TEST_F(StateManager, sleep) {
    core::stateManager::sleep = true;

    core::tick();
    EXPECT_STREQ(HAL::Display::line2, "     SLEEP      ");

    check_idle();
}

TEST_F(StateManager, in_stand) {
    HAL::Tip::in_stand = true;

    core::tick();
    EXPECT_STREQ(HAL::Display::line2, "     STAND      ");

    check_idle();
}

TEST_F(StateManager, configuration_incorrect) {
    core::stateManager::configuration_correct = false;

    core::tick();
    EXPECT_STREQ(HAL::Display::line2, "   CONNECT PC   ");

    check_idle();
}

TEST_F(StateManager, state_running) {
    core::settings.tipParams.offset = 0;
    core::settings.tipParams.gain = 1;

    core::power = 0;
    core::temp = 0;
    core::persistent_state.target = 0;
    core::display::tick();
    EXPECT_STREQ(HAL::Display::line1, "  0/  0 *     0%");
    EXPECT_STREQ(HAL::Display::line2, "                ");

    core::power = 6.249;
    core::temp = 999;
    core::persistent_state.target = 999;
    core::display::tick();
    EXPECT_STREQ(HAL::Display::line1, "999/999 *     6%");
    EXPECT_STREQ(HAL::Display::line2, "                ");

    core::power = 6.25;
    core::temp = 1;
    core::persistent_state.target = 150;
    core::display::tick();
    EXPECT_STREQ(HAL::Display::line1, "  1/150 *     6%");
    EXPECT_STREQ(HAL::Display::line2, "#               ");

    core::power = 6.99;
    core::temp = 87;
    core::persistent_state.target = 56;
    core::display::tick();
    EXPECT_STREQ(HAL::Display::line1, " 87/ 56 *     6%");
    EXPECT_STREQ(HAL::Display::line2, "#               ");

    core::power = 7;
    core::temp = 60;
    core::persistent_state.target = 101;
    core::display::tick();
    EXPECT_STREQ(HAL::Display::line1, " 60/101 *     7%");
    EXPECT_STREQ(HAL::Display::line2, "#               ");

    core::power = 12.499;
    core::display::tick();
    EXPECT_STREQ(HAL::Display::line1, " 60/101 *    12%");
    EXPECT_STREQ(HAL::Display::line2, "#               ");

    core::power = 12.5;
    core::display::tick();
    EXPECT_STREQ(HAL::Display::line1, " 60/101 *    12%");
    EXPECT_STREQ(HAL::Display::line2, "##              ");

    core::power = 18.749;
    core::display::tick();
    EXPECT_STREQ(HAL::Display::line1, " 60/101 *    18%");
    EXPECT_STREQ(HAL::Display::line2, "##              ");

    core::power = 18.75;
    core::display::tick();
    EXPECT_STREQ(HAL::Display::line1, " 60/101 *    18%");
    EXPECT_STREQ(HAL::Display::line2, "###             ");

    core::power = 24.99;
    core::display::tick();
    EXPECT_STREQ(HAL::Display::line1, " 60/101 *    24%");
    EXPECT_STREQ(HAL::Display::line2, "###             ");

    core::power = 25;
    core::display::tick();
    EXPECT_STREQ(HAL::Display::line1, " 60/101 *    25%");
    EXPECT_STREQ(HAL::Display::line2, "####            ");

    core::power = 99.99;
    core::display::tick();
    EXPECT_STREQ(HAL::Display::line1, " 60/101 *    99%");
    EXPECT_STREQ(HAL::Display::line2, "############### ");

    core::power = 100;
    core::display::tick();
    EXPECT_STREQ(HAL::Display::line1, " 60/101 *   100%");
    EXPECT_STREQ(HAL::Display::line2, "################");
}