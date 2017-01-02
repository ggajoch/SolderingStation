#include <cstdio>
#include "gtest/gtest.h"
#include "CLI.h"
#include "HALmock.h"
#include "core.h"
#include "display.h"

bool parse(const char* cmd) {
    static char tmp[100];
    strcpy(tmp, cmd);
    EXPECT_TRUE(libs::CLI::parse_line(tmp));
}

class Commands : public ::testing::Test {
    void SetUp() {
        core::setup();
    }
};

TEST_F(Commands, setTemperature) {
    parse("temp 100");
    EXPECT_FLOAT_EQ(core::target, 100);
    parse("temp 101.5");
    EXPECT_FLOAT_EQ(core::target, 101.5);
    parse("temp 666.666");
    EXPECT_FLOAT_EQ(core::target, 666.666);
}

TEST_F(Commands, setPIDCoefficients) {
    parse("pid 1 2 3");
    EXPECT_FLOAT_EQ(core::pid.params.Kp, 1);
    EXPECT_FLOAT_EQ(core::pid.params.Ki, 2);
    EXPECT_FLOAT_EQ(core::pid.params.Kd, 3);

    parse("pid 33.3 22.2 11.1");
    EXPECT_FLOAT_EQ(core::pid.params.Kp, 33.3);
    EXPECT_FLOAT_EQ(core::pid.params.Ki, 22.2);
    EXPECT_FLOAT_EQ(core::pid.params.Kd, 11.1);
}

TEST_F(Commands, setTipScaling) {
    parse("tip 1 2");
    EXPECT_FLOAT_EQ(core::tempSensor::params.offset, 1);
    EXPECT_FLOAT_EQ(core::tempSensor::params.gain, 2);

    parse("tip 123.456 789.321");
    EXPECT_FLOAT_EQ(core::tempSensor::params.offset, 123.456);
    EXPECT_FLOAT_EQ(core::tempSensor::params.gain, 789.321);
}

TEST_F(Commands, sendConfig) {
    char buf[100];
    std::sprintf(buf,
        "conf %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f\n",
        core::target,
        core::pid.params.Kp,
        core::pid.params.Ki,
        core::pid.params.Kd,
        core::tempSensor::params.offset,
        core::tempSensor::params.gain,
        core::display::backlight,
        core::display::contrast);

    parse("conf");
    EXPECT_TRUE(HAL::Com::checkLastLine(buf));
}

TEST_F(Commands, display) {
    parse("disp 5 7");
    EXPECT_FLOAT_EQ(HAL::Display::backlight, 5);
    EXPECT_FLOAT_EQ(HAL::Display::contrast, 7);

    parse("disp 752.146 749.421");
    EXPECT_FLOAT_EQ(HAL::Display::backlight, 752.146);
    EXPECT_FLOAT_EQ(HAL::Display::contrast, 749.421);
}

TEST_F(Commands, ping) {
    parse("ping");
    EXPECT_TRUE(HAL::Com::checkLastLine("ping\n"));
}