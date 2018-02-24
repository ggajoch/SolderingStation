#pragma once

#include <cstdint>

namespace core {
namespace stateManager {

enum class State {
    InvalidConfig,
    TipError,
    Off,
    Sleep,
    InStand,
    On,

    Count,
};

extern State state;

void init();
void tick();

// clang-format off
enum class Command : std::uint8_t {
    Pid                 = 0b00001,
    Tip                 = 0b00010,
    Display             = 0b00100,
    StandbyTemperatures = 0b01000,
    Timeouts            = 0b10000,

    Correct             = 0b11111,
};
// clang-format on

void config_command_received(Command cmd);

}  // namespace stateManager
}  // namespace core
