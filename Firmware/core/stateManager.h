#pragma once

#include <cstdint>

namespace core {
namespace stateManager {

extern bool sleep, in_stand, configuration_correct;

void tick();

enum class Command : std::uint8_t {
    Pid = 0b001,
    Tip = 0b010,
    Display = 0b100,

    Correct = 0b111,
};

void config_command_received(Command cmd);

}  // namespace stateManager
}  // namespace core
