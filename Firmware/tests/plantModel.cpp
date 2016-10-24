#include <gtest/gtest.h>
#include "plantModel.h"

constexpr double eps = 1e-5;

TEST(plantModel, init) {
    TipModel tip;
    EXPECT_NEAR(tip.Tjoint, tip.Tboard, eps);
    EXPECT_NEAR(tip.Ttip, tip.Tboard, eps);

    for (int i = 0; i < 10; ++i) {
        tip.tick(0);
        EXPECT_NEAR(tip.Ttip, tip.Tboard, eps);
    }

    tip.soldering(false);

    double lastTip = tip.Ttip;
    double lastJoint = tip.Ttip;

    // 5 sec
    for (int i = 0; i < 50; ++i) {
        tip.tick(100);
//        std::cout << tip.Ttip << ", " << tip.Tjoint << std::endl;
        EXPECT_GT(tip.Ttip, lastTip);
        EXPECT_GT(tip.Ttip, tip.Tjoint);
        EXPECT_EQ(tip.Tjoint, lastJoint);

        lastTip = tip.Ttip;
        lastJoint = tip.Tjoint;
    }
    float tipheated = tip.Ttip;
    EXPECT_GT(tip.Ttip, 400);
    EXPECT_NEAR(tip.Tboard, 20, eps);

//    std::cout << "----- soldering -----\n";
    tip.soldering(true);
    // 5 sec
    for (int i = 0; i < 50; ++i) {
        tip.tick(100);
//        std::cout << tip.Ttip << ", " << tip.Tjoint << std::endl;
        EXPECT_GT(tip.Tjoint, lastJoint);
        EXPECT_GT(tipheated, tip.Ttip);

        lastTip = tip.Ttip;
        lastJoint = tip.Tjoint;
    }


//    std::cout << "----- cooling -----\n";
    tip.soldering(false);
    // 5 sec
    for (int i = 0; i < 50; ++i) {
        tip.tick(0);
//        std::cout << tip.Ttip << ", " << tip.Tjoint << std::endl;
        EXPECT_LT(tip.Ttip, lastTip);
        EXPECT_LT(tip.Tjoint, lastJoint);

        lastTip = tip.Ttip;
        lastJoint = tip.Tjoint;
    }
}
