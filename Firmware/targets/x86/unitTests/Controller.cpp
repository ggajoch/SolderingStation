#include <stateManager.h>
#include "gtest/gtest.h"

#include "HALmock.h"
#include "config.h"
#include "core.h"

TEST(Controller, setup) {
    HAL::Tip::heatingPercentage = 10;
    core::setup();
    EXPECT_FLOAT_EQ(HAL::Tip::heatingPercentage, 0);
}

void clear() {
    while (!HAL::Tip::rawTemperatureData.empty())
        HAL::Tip::rawTemperatureData.pop();
}

static void parse(const char* cmd) {
    static char tmp[100];
    strcpy(tmp, cmd);
    HAL::Com::handler(tmp);
}

TEST(Controller, averaging) {
    clear();

    core::settings.tipParams.offset = 0;
    core::settings.tipParams.gain = 1;

    for (int i = 0; i < core::config::tempAverages; ++i) {
        HAL::Tip::rawTemperatureData.push(i % 2);
    }
    core::tick();

    EXPECT_EQ(HAL::Tip::rawTemperatureData.size(), 1); // leftover for testing
    EXPECT_FLOAT_EQ(core::temp, 0.5);

    clear();

    HAL::Tip::rawTemperatureData.push(50);
    for (int i = 1; i < core::config::tempAverages; ++i) {
        HAL::Tip::rawTemperatureData.push(i);
    }
    EXPECT_EQ(HAL::Tip::rawTemperatureData.size(), 100);

    core::tick();

    EXPECT_EQ(HAL::Tip::rawTemperatureData.size(), 1); // leftover for testing
    EXPECT_FLOAT_EQ(core::temp, 50.0);
}


TEST(Controller, blank_memory_setup) {
    std::fill(HAL::Memory::table.begin(), HAL::Memory::table.end(), 0xFF);
    HAL::Tip::in_stand = true;

    core::setup();

    HAL::Encoder::buttonPressedHandler();
    core::tick();
    HAL::Encoder::buttonReleasedHandler();
    core::tick();


    parse("temp 200");
    core::tick();

    using namespace core::stateManager;

    EXPECT_EQ(State::InvalidConfig, state);

    parse("pid 0.1 0.2 0.3 0.4");
    core::tick();

    EXPECT_EQ(State::InvalidConfig, state);

    parse("disp 10.1 20.2");
    core::tick();

    EXPECT_EQ(State::InvalidConfig, state);

    parse("tip 1.1 2.2");
    core::tick();

    EXPECT_EQ(State::InvalidConfig, state);

    parse("stdby 17.9 100");
    core::tick();

    EXPECT_EQ(State::InvalidConfig, state);

    parse("timeouts 10 255");
    core::tick();

    EXPECT_EQ(State::Off, state);

    HAL::Encoder::buttonPressedHandler();
    for(int i = 0; i < 100; ++i) {
        core::tick();
    }
    HAL::Encoder::buttonReleasedHandler();
    core::tick();

    EXPECT_EQ(State::InStand, state);
    EXPECT_FLOAT_EQ(core::pid.target, 100.0);

    HAL::Tip::in_stand = false;
    core::tick();

    EXPECT_EQ(State::On, state);
    EXPECT_FLOAT_EQ(core::pid.target, 200.0);

    HAL::Encoder::buttonPressedHandler();
    HAL::Encoder::buttonReleasedHandler();
    core::tick();

    EXPECT_EQ(State::Sleep, state);
    EXPECT_FLOAT_EQ(core::pid.target, 17);
}