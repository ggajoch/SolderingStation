#include <cstdio>
#include "gtest/gtest.h"
#include "CLI.h"
#include "HALmock.h"
#include "core.h"
#include "display.h"
#include "settings.h"
#include "storage/persistent_state.h"

void parse(const char* cmd) {
    static char tmp[100];
    strcpy(tmp, cmd);
    EXPECT_TRUE(libs::CLI::parse_line(tmp));
}

void parse_tick(const char* cmd) {
    static char tmp[100];
    strcpy(tmp, cmd);
    HAL::Com::handler(tmp);
    core::tick();
}

class Commands : public ::testing::Test {
    void SetUp() {
        core::setup();
    }
};

TEST_F(Commands, noCommandNoFail) {
    libs::CLI::set_commands({});
    core::persistent_state.target = 100;
    core::settings.pidParams.Kp =  33.3;
    core::settings.pidParams.Ki =  22.2;
    core::settings.pidParams.Kd =  11.1;
    core::settings.tipParams.offset = 1;
    core::settings.tipParams.gain = 2;

    auto parse_fail = [](const char* cmd) {
        static char tmp[100];
        strcpy(tmp, cmd);
        EXPECT_FALSE(libs::CLI::parse_line(tmp));
    };
    parse_fail("temp 100");
    parse_fail("pid 33.3 22.2 11.1");
    parse_fail("tip 123.456 789.321");
    parse_fail("conf");
    parse_fail("disp 5 7");
    parse_fail("ping");

    EXPECT_EQ(core::persistent_state.target, 100);
    EXPECT_FLOAT_EQ(core::settings.pidParams.Kp, 33.3);
    EXPECT_FLOAT_EQ(core::settings.pidParams.Ki, 22.2);
    EXPECT_FLOAT_EQ(core::settings.pidParams.Kd, 11.1);
    EXPECT_FLOAT_EQ(core::settings.tipParams.offset, 1);
    EXPECT_FLOAT_EQ(core::settings.tipParams.gain, 2);
}

TEST_F(Commands, setTemperature) {
    for(auto foo : {parse, parse_tick}) {
        foo("temp 100");
        EXPECT_EQ(core::persistent_state.target, 100);
        foo("temp 101");
        EXPECT_EQ(core::persistent_state.target, 101);
        foo("temp 666");
        EXPECT_EQ(core::persistent_state.target, 666);
    }
}

TEST_F(Commands, setPIDCoefficients) {
    for(auto foo : {parse, parse_tick}) {
        foo("pid 1 2 3 47");
        EXPECT_FLOAT_EQ(core::settings.pidParams.Kp, 1);
        EXPECT_FLOAT_EQ(core::settings.pidParams.Ki, 2);
        EXPECT_FLOAT_EQ(core::settings.pidParams.Kd, 3);
        EXPECT_FLOAT_EQ(core::settings.pidParams.max_power, 47);

        foo("pid 33.3 22.2 11.1 123");
        EXPECT_FLOAT_EQ(core::settings.pidParams.Kp, 33.3);
        EXPECT_FLOAT_EQ(core::settings.pidParams.Ki, 22.2);
        EXPECT_FLOAT_EQ(core::settings.pidParams.Kd, 11.1);
        EXPECT_FLOAT_EQ(core::settings.pidParams.max_power, 123);
    }
}

TEST_F(Commands, setTipScaling) {
    for(auto foo : {parse, parse_tick}) {
        foo("tip 1 2");
        EXPECT_FLOAT_EQ(core::settings.tipParams.offset, 1);
        EXPECT_FLOAT_EQ(core::settings.tipParams.gain, 2);

        foo("tip 123.456 789.321");
        EXPECT_FLOAT_EQ(core::settings.tipParams.offset, 123.456);
        EXPECT_FLOAT_EQ(core::settings.tipParams.gain, 789.321);
    }
}

TEST_F(Commands, display) {
    for(auto foo : {parse, parse_tick}) {
        foo("disp 5 7");
        EXPECT_FLOAT_EQ(HAL::Display::backlight, 5);
        EXPECT_FLOAT_EQ(HAL::Display::contrast, 7);

        foo("disp 752.146 749.421");
        EXPECT_FLOAT_EQ(HAL::Display::backlight, 752.146);
        EXPECT_FLOAT_EQ(HAL::Display::contrast, 749.421);
    }
}

TEST_F(Commands, timeouts) {
    for(auto foo : {parse, parse_tick}) {
        foo("timeouts 255 0");
        EXPECT_EQ(255, core::settings.timeouts.sleep);
        EXPECT_EQ(0, core::settings.timeouts.off);

        foo("timeouts 58 127");
        EXPECT_EQ(58, core::settings.timeouts.sleep);
        EXPECT_EQ(127, core::settings.timeouts.off);
    }
}


TEST_F(Commands, ping) {
    parse("ping");
    HAL::Com::checkLastLine("pong\n");
}
