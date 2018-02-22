#include <algorithm>

#include "gtest/gtest.h"

#include "pid.h"

constexpr float eps = 1e-2;

void reset() {
    core::settings.pidParams.Kp = 0;
    core::settings.pidParams.Ki = 0;
    core::settings.pidParams.Kd = 0;
    core::settings.pidParams.max_power = 100;
}

TEST(PID, setup) {
    core::PID pid;
    reset();
    EXPECT_NEAR(pid.target, 0, eps);
    EXPECT_NEAR(core::settings.pidParams.Kp, 0, eps);
    EXPECT_NEAR(core::settings.pidParams.Ki, 0, eps);
    EXPECT_NEAR(core::settings.pidParams.Kd, 0, eps);
    EXPECT_NEAR(pid.integral, 0, eps);
    EXPECT_NEAR(pid.prevError, 0, eps);
}

TEST(PID, noOutput) {
    core::PID pid;
    EXPECT_NEAR(pid.tick(0), 0, eps);
    EXPECT_NEAR(pid.tick(100), 0, eps);
}

TEST(PID, proportional) {
    core::PID pid;
    reset();
    core::settings.pidParams.Kp = -1;
    pid.target = -0.5f;
    pid.lowerLimit = -1e6f;
    pid.upperLimit = 1e6f;
    EXPECT_NEAR(pid.tick(0), 0, eps);
    for (float i = -100; i <= 100; i += 0.7) {
        EXPECT_NEAR(pid.tick(i), i, eps);
    }

    core::settings.pidParams.Kp = 1.5f;
    pid.target = 9.5f;
    EXPECT_NEAR(pid.tick(10), 0, eps);
    for (float i = -10; i <= 10; i += 0.07) {
        EXPECT_NEAR(pid.tick(i), -1.5 * (i - 10), eps);
    }
}

TEST(PID, integral) {
    core::PID pid;
    reset();
    core::settings.pidParams.Ki = -1.0f/1e3f;
    pid.target = -0.5f;
    pid.lowerLimit = -1e6f;
    pid.upperLimit = 1e6f;
    EXPECT_NEAR(pid.tick(0), 0, eps);
    for (int i = 1; i <= 500; ++i) {
        EXPECT_NEAR(pid.tick(1)*1e3, i, eps);
    }
}

TEST(PID, derivative) {
    core::PID pid;
    reset();
    core::settings.pidParams.Kd = -1.0f;
    pid.target = -0.5f;
    pid.lowerLimit = -1e6f;
    pid.upperLimit = 1e6f;
    EXPECT_NEAR(pid.tick(0), 0, eps);

    for (int i = 1; i <= 50; ++i) {
        EXPECT_NEAR(pid.tick(0), -(i - 1), eps);
        EXPECT_NEAR(pid.tick(i), i, eps);
    }
}

TEST(PID, limits) {
    core::PID pid;
    reset();
    pid.target = -0.5f;
    core::settings.pidParams.Kp = -1.0f;
    pid.lowerLimit = -17;
    pid.upperLimit = 85;
    EXPECT_NEAR(pid.tick(0), 0, eps);

    for (float i = -100; i <= 100; i += 0.7) {
        float expected = std::max(-17.0f, std::min(i, 85.0f));
        EXPECT_NEAR(pid.tick(i), expected, eps);
    }
}

TEST(PID, antiWindup) {
    core::PID pid;
    reset();
    core::settings.pidParams.Kp = 1.0f;
    core::settings.pidParams.Ki = 1.0f;
    pid.target = 9.5;
    pid.lowerLimit = 0;
    pid.upperLimit = 100;

    float temp = 0;
    for (int i = 1; i <= 100; ++i) {
        pid.tick(0);
    }
    for (int i = 1; i <= 100; ++i) {
        float pwr = pid.tick(temp);
        //        printf("%f\t%f\t%f\n", pwr, temp, pid.integral);
        EXPECT_GT(pwr, 99.0f);
        EXPECT_LT(pid.integral, pid.upperLimit);
        EXPECT_GT(pid.integral, pid.lowerLimit);

        temp += 0.1;
    }
}

TEST(PID, powerLimit) {
    core::PID pid;
    reset();
    core::settings.pidParams.Kp = -1;
    core::settings.pidParams.max_power = 15;
    pid.target = -0.5f;
    pid.lowerLimit = -1e6f;
    pid.upperLimit = 1e6f;
    EXPECT_NEAR(pid.tick(0), 0, eps);
    for (float i = -500; i <= 500; i += 0.7) {
        float expected = std::min(i, 15.0f);
        EXPECT_NEAR(pid.tick(i), expected, eps);
    }
}