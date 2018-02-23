#include <gtest/gtest.h>

#include "pid.h"
#include "plantModel.h"

constexpr double eps = 1e-5;

TEST(plantTip, simplesoldering) {
    TipModel model;
    core::PID pid;

    pid.target = 299.5;
    core::settings.pidParams.Kp = 10;
    core::settings.pidParams.Ki = 4;
    core::settings.pidParams.Kd = 0;
    core::settings.pidParams.max_power = 100.0;
    pid.upperLimit = 100;
    pid.integral = 0;

    model.soldering(false);
    for (int sec = 0; sec < 10; sec++) {
        for (int tick = 0; tick < 10; tick++) {
            float pwr = pid.tick(model.Ttip);
            model.tick(pwr);
            //            std::printf("%f\t%f\t%f\n", model.Ttip, pwr, pid.integral);
        }
    }

    EXPECT_NEAR(model.Ttip, 300, 0.1);

    pid.target = 349.5;

    for (int sec = 0; sec < 10; sec++) {
        for (int tick = 0; tick < 10; tick++) {
            float pwr = pid.tick(model.Ttip);
            model.tick(pwr);
            //            std::printf("%f\t%f\t%f\n", model.Ttip, pwr, pid.integral);
        }
    }

    EXPECT_NEAR(model.Ttip, 350, 0.1);

    model.soldering(true);

    for (int sec = 0; sec < 10; sec++) {
        for (int tick = 0; tick < 20; tick++) {
            float pwr = pid.tick(model.Ttip);
            model.tick(pwr);
            //            std::printf("%f\t%f\t%f\n", model.Ttip, pwr, pid.integral);
        }
    }

    EXPECT_NEAR(model.Ttip, 350, 0.1);
}
