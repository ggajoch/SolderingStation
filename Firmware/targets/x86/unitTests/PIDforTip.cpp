#include <gtest/gtest.h>

#include "pid.h"
#include "plantModel.h"

constexpr double eps = 1e-5;

TEST(plantTip, simplesoldering) {
    TipModel model;
    core::PID pid;

    pid.target = 300;
    pid.params.Kp = 10;
    pid.params.Ki = 4;
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

    pid.target = 350;

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
