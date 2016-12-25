#include <gtest/gtest.h>

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    auto r = RUN_ALL_TESTS();

    return r;
}
