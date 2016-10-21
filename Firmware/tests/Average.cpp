#include "gtest/gtest.h"
#include "Average.h"

constexpr float eps = 1e-4;

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

TEST(AverageTest, oneHundredPoints) {
    libs::Average<float, 100> average;
    EXPECT_NEAR(average.get(), 0, eps);
    for (int i = 1; i < 1000; ++i) {
        average.put(1570);
        EXPECT_NEAR(average.get(), std::min(1570.0*i/100.0, 1570.0), eps);
    }
}

TEST(AverageTest, interleaving) {
    libs::Average<float, 150> average;
    EXPECT_NEAR(average.get(), 0, eps);

    for (int i = 0; i < 1000; ++i) {
        average.put(i);
        average.put(-i);

        EXPECT_NEAR(average.get(), 0, eps);
    }
}
