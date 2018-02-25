#include <array>
#include <chrono>
#include <optional>

#include "HAL.h"
#include "core.h"
#include "stateManager.h"
#include "tempSensor.h"
#include "timer.h"

using namespace std::chrono_literals;

namespace core::stateManager {

State state;
bool in_stand;

enum class Event {
    ConfigurationReceived,
    ButtonPressed,
    ButtonHeld,
    AnyUserAction,
    TimeoutToSleep,
    TimeoutToOff,

    // actions below are invoked in every iteration
    // as transition is defined by state not by edge
    InStand,
    NotInStand,

    TemperatureOK,
    TemperatureFail,

    Count,
};

namespace transitions {

constexpr static auto InvalidConfig = State::InvalidConfig;
constexpr static auto TipError = State::TipError;
constexpr static auto Off = State::Off;
constexpr static auto Sleep = State::Sleep;
constexpr static auto InStand = State::InStand;
constexpr static auto On = State::On;

// clang-format off
std::array<std::array<State, static_cast<int>(State::Count)>, static_cast<int>(Event::Count)> transitions = {{
//                         InvalidConfig, TipError, Off,      Sleep,    InStand,   On
//                        {-------------, --------, --------, --------, --------, --------}
//                        {             ,         ,         ,         ,         ,         }
/*ConfigurationReceived*/ {Off          , TipError, Off     , Sleep   , InStand , On      },
/*    ButtonPressed    */ {InvalidConfig, TipError, Off     , On      , Sleep   , Sleep   },
/*     ButtonHeld      */ {InvalidConfig, TipError, On      , Off     , Off     , Off     },
/*    AnyUserAction    */ {InvalidConfig, TipError, Off     , On      , InStand , On      },
/*    TimeoutToSleep   */ {InvalidConfig, TipError, Off     , Sleep   , Sleep   , Sleep   },
/*     TimeoutToOff    */ {InvalidConfig, TipError, Off     , Off     , Off     , Off     },
/*       InStand       */ {InvalidConfig, TipError, Off     , Sleep   , InStand , InStand },
/*      NotInStand     */ {InvalidConfig, TipError, Off     , Sleep   , On      , On      },
/*     TemperatureOK   */ {InvalidConfig, Off     , Off     , Sleep   , InStand , On      },
/*    TemperatureFail  */ {InvalidConfig, TipError, TipError, TipError, TipError, TipError},
}};
// clang-format on

}  // namespace transitions

std::array<Event, 10> events;
uint8_t events_to_process;

void add_event(Event event) {
    if (events_to_process < events.size()) {
        events[events_to_process++] = event;
    }
}

void process_events() {
    for (int i = 0; i < events_to_process; ++i) {
        state = transitions::transitions[static_cast<unsigned>(events[i])][static_cast<unsigned>(state)];
    }
    events_to_process = 0;
}

template <typename T>
class ChangeObserver {
    T last_val;
    void (*foo)();

 public:
    explicit constexpr ChangeObserver(void (*foo)()) noexcept : foo{foo} {
    }

    void check(T val) {
        if (last_val != val) {
            foo();
            last_val = val;
        }
    }
};

std::chrono::milliseconds last_action_time;
void reset_timeouts() {
    last_action_time = timer::now();
}
void process_timeouts() {
    if (settings.timeouts.sleep != 0 && timer::now() > last_action_time + std::chrono::minutes(settings.timeouts.sleep)) {
        add_event(Event::TimeoutToSleep);
    }
    if (settings.timeouts.off != 0 && timer::now() > last_action_time + std::chrono::minutes(settings.timeouts.off)) {
        add_event(Event::TimeoutToOff);
    }
}

bool button_pressed, button_released;
static void process_button() {
    static std::optional<std::chrono::milliseconds> button_hold_timeout;

    if (button_pressed) {
        button_pressed = false;
        reset_timeouts();
        button_hold_timeout = core::timer::now() + 500ms;
    }

    if (button_released) {
        reset_timeouts();
        button_released = false;
        if (button_hold_timeout) {
            button_hold_timeout = {};

            add_event(Event::ButtonPressed);
        }
    }

    if (button_hold_timeout && button_hold_timeout < core::timer::now()) {
        button_hold_timeout = {};
        add_event(Event::ButtonHeld);
    }
}

void any_action() {
    add_event(Event::AnyUserAction);
    reset_timeouts();
}

static void process_user_events() {
    static ChangeObserver<uint16_t> target_observer(any_action);
    static ChangeObserver<bool> in_stand_observer(any_action);

    target_observer.check(core::persistent_state.target);

    in_stand = HAL::Tip::inStand();
    in_stand_observer.check(in_stand);

    process_button();

    add_event(in_stand ? Event::InStand : Event::NotInStand);
    add_event(core::tempSensor::temperatureInLimits() ? Event::TemperatureOK : Event::TemperatureFail);

    process_timeouts();
}

static void set_temperature() {
    static ChangeObserver<State> state_observer([]() { pid.reset(); });
    state_observer.check(state);

    switch (state) {
        case State::InvalidConfig:
        case State::TipError:
        case State::Off:
        default:
            core::pid.target = 0.0f;
            break;

        case State::Sleep:
            core::pid.target = core::settings.sleep_temperature;
            break;

        case State::InStand:
            core::pid.target = core::settings.stand_temperature;
            break;

        case State::On:
            core::pid.target = core::persistent_state.target;
            break;
    }
}

static uint8_t config_send_from_pc;

void init() {
    config_send_from_pc = 0;
}

void tick() {
    process_user_events();

    process_events();

    set_temperature();
}

void config_command_received(Command cmd) {
    config_send_from_pc |= static_cast<uint8_t>(cmd);

    if (config_send_from_pc == static_cast<uint8_t>(Command::Correct)) {
        add_event(Event::ConfigurationReceived);
    }
}

}  // namespace core::stateManager

namespace HAL::Encoder {

void buttonPressedHandler() {
    core::stateManager::button_pressed = true;
}

void buttonReleasedHandler() {
    core::stateManager::button_released = true;
}

}  // namespace HAL::Encoder
