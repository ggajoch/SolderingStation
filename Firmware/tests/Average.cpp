#include "gtest/gtest.h"
#include "Average.h"

constexpr float eps = 1e-5;

TEST(AverageTest, two_points) {
    libs::Average<float, 2> average;
    EXPECT_NEAR(average.get(), 0, eps);

    average.put(1);
    EXPECT_NEAR(average.get(), 0.5, eps);

    average.put(1);
    EXPECT_NEAR(average.get(), 1, eps);

    average.put(2);
    EXPECT_NEAR(average.get(), 1.5, eps);

    average.put(0);
    EXPECT_NEAR(average.get(), 1, eps);

    average.put(100);
    EXPECT_NEAR(average.get(), 50, eps);

    average.put(100);
    EXPECT_NEAR(average.get(), 100, eps);

    average.put(100);
    EXPECT_NEAR(average.get(), 100, eps);
}