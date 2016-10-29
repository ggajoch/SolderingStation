#include "gtest/gtest.h"
#include "array_view.h"

TEST(array_view, check_values) {
    constexpr int size = 10;
    int tab[size];
    for (int i = 0; i < size; ++i) {
        tab[i] = i;
    }
    libs::array_view<int> arv(tab);
    const libs::array_view<int> carv(tab);

    EXPECT_NE(arv.data(), nullptr);
    EXPECT_EQ(arv.size(), 10);
    EXPECT_EQ(arv.size_bytes(), 40);
    EXPECT_NE(carv.data(), nullptr);
    EXPECT_EQ(carv.size(), 10);
    EXPECT_EQ(carv.size_bytes(), 40);

    for (int i = 0; i < size; ++i) {
        EXPECT_EQ(arv[i], i);
        EXPECT_EQ(carv[i], i);
        EXPECT_EQ(arv.at(i), i);
        EXPECT_EQ(carv.at(i), i);
    }

    EXPECT_NE(arv.data(), nullptr);
    EXPECT_EQ(arv.size(), 10);
    EXPECT_EQ(arv.size_bytes(), 40);
    EXPECT_NE(carv.data(), nullptr);
    EXPECT_EQ(carv.size(), 10);
    EXPECT_EQ(carv.size_bytes(), 40);

    int i = 0;
    for (auto &x : arv) {
        EXPECT_EQ(x, i++);
    }
    i = 0;
    for (const auto &x : carv) {
        EXPECT_EQ(x, i++);
    }

    EXPECT_NE(arv.data(), nullptr);
    EXPECT_EQ(arv.size(), 10);
    EXPECT_EQ(arv.size_bytes(), 40);
    EXPECT_NE(carv.data(), nullptr);
    EXPECT_EQ(carv.size(), 10);
    EXPECT_EQ(carv.size_bytes(), 40);
}

TEST(array_view, equal) {
    constexpr int size = 10;
    int tab[size];
    for (int i = 0; i < size; ++i) {
        tab[i] = i;
    }
    libs::array_view<int> arv(tab);
    libs::array_view<int> arv2;
    EXPECT_EQ(arv.empty(), false);
    EXPECT_EQ(arv2.empty(), true);
    EXPECT_EQ(arv == arv2, false);
    EXPECT_EQ(arv != arv2, true);
    EXPECT_EQ(arv == nullptr, false);
    EXPECT_EQ(arv2 == nullptr, true);
    arv2 = arv;
    EXPECT_EQ(arv == arv2, true);
    EXPECT_EQ(arv != arv2, false);
    EXPECT_EQ(arv == nullptr, false);
    EXPECT_EQ(arv2 == nullptr, false);
    EXPECT_EQ(arv.empty(), false);
    EXPECT_EQ(arv2.empty(), false);

    for (int i = 0; i < size; ++i) {
        EXPECT_EQ(arv[i], i);
        EXPECT_EQ(arv2[i], i);
        EXPECT_EQ(arv.at(i), i);
        EXPECT_EQ(arv2.at(i), i);
    }
}

TEST(array_view, reset) {
    constexpr int size = 100;
    int tab[size];
    for (int i = 0; i < size; ++i) {
        tab[i] = i;
    }
    libs::array_view<int32_t> arv(tab);

    for (int i = 0; i < size; ++i) {
        EXPECT_EQ(arv[i], i);
    }
    EXPECT_NE(arv.data(), nullptr);
    EXPECT_EQ(arv.size(), 100);
    EXPECT_EQ(arv.size_bytes(), 400);

    arv.reset();
    EXPECT_EQ(arv.data(), nullptr);
    EXPECT_EQ(arv.size(), 0);
    EXPECT_EQ(arv.size_bytes(), 0);
}

TEST(array_view, pointers) {
    constexpr int size = 10;
    int tab[size];
    for (int i = 0; i < size; ++i) {
        tab[i] = i;
    }
    libs::array_view<int32_t> arv(tab);
    const libs::array_view<int32_t> carv(arv);

    int32_t * ptr = arv.data();
    EXPECT_EQ(ptr, arv.data());
    const int32_t * cptr = carv.data();
    EXPECT_EQ(cptr, carv.data());
}

TEST(array_view, references) {
    constexpr int size = 10;
    int tab[size];
    for (int i = 0; i < size; ++i) {
        tab[i] = i;
    }
    libs::array_view<int32_t> arv(tab);
    const libs::array_view<int32_t> carv(tab);

    EXPECT_EQ(&arv[0], arv.begin());
    EXPECT_EQ(&arv[0]+10, arv.end());
    EXPECT_EQ(&carv[0], carv.begin());
    EXPECT_EQ(&carv[0]+10, carv.end());

    EXPECT_EQ(arv[0], arv.front());
    EXPECT_EQ(arv[9], arv.back());
    EXPECT_EQ(carv[0], arv.front());
    EXPECT_EQ(carv[9], arv.back());
}

TEST(array_view, slice) {
    constexpr int size = 100;
    int tab[size];
    for (int i = 0; i < size; ++i) {
        tab[i] = i;
    }
    int * const tab_ptr = tab;
    libs::array_view<int32_t> arv(tab_ptr, 100);

    libs::array_view<int32_t> arv2(arv.slice(50));

    EXPECT_EQ(arv2.size(), 50);
    for (int i = 0; i < arv2.size(); ++i) {
        EXPECT_EQ(arv2[i], i+50);
    }

    arv2 = arv.slice(20, 30);

    EXPECT_EQ(arv2.size(), 10);
    for (int i = 0; i < arv2.size(); ++i) {
        EXPECT_EQ(arv2[i], i+20);
    }
}
