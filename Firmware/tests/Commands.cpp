#include "gtest/gtest.h"

#include "CLI.h"
#include "Controller.h"

constexpr float eps = 1e-4;

static void parse(const char * cmd) {
    static char tmp[100];
    strcpy(tmp, cmd);
    libs::CLI::parse_line(tmp);
}

TEST(Commands, temp) {
    parse("temp 100");
    EXPECT_NEAR(core::controller.pid.target, 100, eps);
    parse("temp 101.5");
    EXPECT_NEAR(core::controller.pid.target, 101.5, eps);
    parse("temp 666.666");
    EXPECT_NEAR(core::controller.pid.target, 666.666, eps);
}

TEST(Commands, pid) {
    parse("pid 1 2 3");
    EXPECT_NEAR(core::controller.pid.Kp, 1, eps);
    EXPECT_NEAR(core::controller.pid.Ki, 2, eps);
    EXPECT_NEAR(core::controller.pid.Kd, 3, eps);

    parse("pid 33.3 22.2 11.1");
    EXPECT_NEAR(core::controller.pid.Kp, 33.3, eps);
    EXPECT_NEAR(core::controller.pid.Ki, 22.2, eps);
    EXPECT_NEAR(core::controller.pid.Kd, 11.1, eps);
}
