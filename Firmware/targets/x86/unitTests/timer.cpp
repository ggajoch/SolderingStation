#include <gtest/gtest.h>

#include "core.h"
#include "timer.h"


TEST(timer, increase) {
    auto now = core::timer::now();
    for(int i = 0; i < 100; ++i) {
        core::tick();
        now += core::config::tickDuration;
        EXPECT_EQ(now, core::timer::now());
    }
}
