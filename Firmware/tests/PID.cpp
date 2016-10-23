#include "gtest/gtest.h"

#include "pid.h"

constexpr float eps = 1e-4;

TEST(PID, setup) {
    core::PID pid;
    EXPECT_NEAR(pid.target, 0, eps);
    EXPECT_NEAR(pid.Kp, 0, eps);
    EXPECT_NEAR(pid.Ki, 0, eps);
    EXPECT_NEAR(pid.Kd, 0, eps);
    EXPECT_NEAR(pid.Kb, 0, eps);
    EXPECT_NEAR(pid.Kp, 0, eps);
    EXPECT_NEAR(pid.integral, 0, eps);
    EXPECT_NEAR(pid.prevError, 0, eps);
    EXPECT_NEAR(pid.prevOutput, 0, eps);
}

TEST(PID, noOutput) {
    core::PID pid;
    EXPECT_NEAR(pid.tick(0), 0, eps);
    EXPECT_NEAR(pid.tick(100), 0, eps);
}


TEST(PID, proportional) {
    core::PID pid;
    pid.Kp = 1;
    pid.target = 0;
    pid.lowerLimit = -1e6f;
    pid.upperLimit = 1e6f;
    EXPECT_NEAR(pid.tick(0), 0, eps);
    for (float i = -500; i <= 500; i += 0.7) {
        EXPECT_NEAR(pid.tick(i), i, eps);
    }


    pid.Kp = -1.5f;
    pid.target = 10.0f;
    EXPECT_NEAR(pid.tick(10), 0, eps);
    for (float i = -500; i <= 500; i += 0.7) {
        EXPECT_NEAR(pid.tick(i), -1.5*(i-10), eps);
    }
}

TEST(PID, limits) {
    core::PID pid;
    pid.Kp = 1;
    pid.lowerLimit = -107;
    pid.upperLimit = 257;
    EXPECT_NEAR(pid.tick(0), 0, eps);

    for (float i = -500; i <= 500; i += 0.7) {
        float expected = std::max(-107.0f, std::min(i, 257.0f));
        EXPECT_NEAR(pid.tick(i), expected, eps);
    }

    pid.lowerLimit = -17;
    pid.upperLimit = 13;
    EXPECT_NEAR(pid.tick(0), 0, eps);

    for (float i = -500; i <= 500; i += 0.7) {
        float expected = std::max(-17.0f, std::min(i, 13.0f));
        EXPECT_NEAR(pid.tick(i), expected, eps);
    }
}
