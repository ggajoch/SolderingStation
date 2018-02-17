#pragma once

#include <cstdint>

namespace core {
namespace stateManager {

extern bool off, sleep, in_stand, configuration_correct;

void tick();

enum class Command : std::uint8_t {
    Pid                 = 0b0001,
    Tip                 = 0b0010,
    Display             = 0b0100,
    StandbyTemperatures = 0b1000,

    Correct             = 0b1111,
};

void config_command_received(Command cmd);

}  // namespace stateManager
}  // namespace core
