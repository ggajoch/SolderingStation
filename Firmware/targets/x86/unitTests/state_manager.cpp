#include <stateManager.h>
#include <timer.h>
#include "gtest/gtest.h"

#include "HALmock.h"
#include "core.h"
#include "display.h"

using namespace core::stateManager;

class StateManager : public ::testing::Test {
    void SetUp() {
        core::setup();

        
        core::persistent_state.target = 1;
        HAL::Tip::set_temperature(60);
        core::tick();

        HAL::Tip::in_stand = false;
        core::settings.pidParams.Kp = 100;
        core::settings.pidParams.max_power = 100;
        core::persistent_state.target = 101;

        HAL::Tip::set_temperature(100);
        core::settings.tipParams.offset = 10;
        core::settings.tipParams.gain = 0.5;

        core::tick();
        core::tick();
    }
};



void send_cmd(const char * line) {
    static char tmp[100];
    strcpy(tmp, line);
    HAL::Com::handler(tmp);
}

TEST_F(StateManager, send_config) {
    send_cmd("pid 0.1 154.23 193.45 14.52");
    core::tick();
    EXPECT_EQ(State::InvalidConfig, state);

    send_cmd("disp 10 0.5");
    core::tick();
    EXPECT_EQ(State::InvalidConfig, state);

    send_cmd("tip 10 0.5");
    core::tick();
    EXPECT_EQ(State::InvalidConfig, state);

    send_cmd("timeouts 1 1");
    core::tick();
    EXPECT_EQ(State::InvalidConfig, state);

    send_cmd("stdby 0 10");
    core::tick();
    EXPECT_EQ(State::Off, state);
}


TEST_F(StateManager, on_off) {
    state = State::Off;

    {
        HAL::Encoder::buttonPressedHandler();
        for(int i = 0; i < 5; ++i) {
            core::tick();
        }

        HAL::Encoder::buttonReleasedHandler();
        core::tick();

        EXPECT_EQ(State::Off, state);
    }

    {
        HAL::Encoder::buttonPressedHandler();
        for(int i = 0; i < 7; ++i) {
            core::tick();
        }

        HAL::Encoder::buttonReleasedHandler();
        core::tick();

        EXPECT_EQ(State::On, state);
    }

    {
        HAL::Encoder::buttonPressedHandler();
        for(int i = 0; i < 7; ++i) {
            core::tick();
        }

        HAL::Encoder::buttonReleasedHandler();
        core::tick();

        EXPECT_EQ(State::Off, state);
    }
}

TEST_F(StateManager, stand_detection) {
    // test spec - from, where if not in stand, where if in stand

    std::initializer_list<std::array<State, 3>> testspec
            {{State::InvalidConfig, State::InvalidConfig, State::InvalidConfig},
             {State::Off,           State::Off,           State::Off},
             {State::Sleep,         State::On,            State::InStand},
             {State::InStand,       State::On,            State::InStand},
             {State::On,            State::On,            State::InStand}};


    for(auto test : testspec) {
        state = test[0];

        HAL::Tip::in_stand = false;
        core::tick();
        EXPECT_EQ(test[1], state);

        state = test[0];

        HAL::Tip::in_stand = true;
        core::tick();
        EXPECT_EQ(test[2], state);
    }
}

TEST_F(StateManager, button_short_press) {
    // test spec - from, where if button short pressed

    std::initializer_list<std::array<State, 2>> testspec =
            {{State::InvalidConfig, State::InvalidConfig},
             {State::Off,           State::Off},
             {State::Sleep,         State::On},
             {State::InStand,       State::Sleep},
             {State::On,            State::Sleep}};


    for(auto test : testspec) {
        state = test[0];

        HAL::Encoder::buttonPressedHandler();
        HAL::Encoder::buttonReleasedHandler();

        core::tick();
        EXPECT_EQ(test[1], state);
    }
}

TEST_F(StateManager, button_long_press) {
    // test spec - from, where if button long pressed

    std::initializer_list<std::array<State, 2>> testspec =
            {{State::InvalidConfig, State::InvalidConfig},
             {State::Off,           State::On},
             {State::Sleep,         State::Off},
             {State::InStand,       State::Off},
             {State::On,            State::Off}};


    for(auto test : testspec) {
        state = test[0];

        HAL::Encoder::buttonPressedHandler();
        for(int i = 0; i < 100; ++i) {
            core::tick();
        }
        HAL::Encoder::buttonReleasedHandler();

        core::tick();
        EXPECT_EQ(test[1], state);
    }
}

namespace core {
namespace stateManager {
extern std::chrono::milliseconds last_action_time;
}
}

using namespace std::chrono_literals;

TEST_F(StateManager, reset_timeout) {
    auto check = [](bool resetting, auto foo) {
        last_action_time = 0ms;

        core::tick();
        core::tick();

        foo();
        auto now = core::timer::now();
        core::tick();

        EXPECT_EQ(resetting?now:0ms, last_action_time);
    };

    check(true, [](){
        HAL::Encoder::buttonPressedHandler();
    });

    check(true, [](){
        HAL::Encoder::buttonReleasedHandler();
    });

    check(true, [](){
        core::persistent_state.target += 5;
    });

    HAL::Tip::in_stand = false;
    check(true, [](){
        HAL::Tip::in_stand = true;
    });

    HAL::Tip::in_stand = true;
    check(true, [](){
        HAL::Tip::in_stand = false;
    });
}

TEST_F(StateManager, timeout) {
    core::settings.timeouts.sleep = 2;
    core::settings.timeouts.off = 3;

    HAL::Encoder::buttonPressedHandler();
    HAL::Encoder::buttonReleasedHandler();
    core::tick();

    state = State::On;

    for (int i = 0; i < 2*60*10; ++i) {
        core::tick();
        EXPECT_EQ(State::On, state);
    }

    for (int i = 0; i < 60*10; ++i) {
        core::tick();
        EXPECT_EQ(State::Sleep, state);
    }

    core::tick();
    EXPECT_EQ(State::Off, state);
}