#include <cstdio>

#include "CLI.h"
#include "Controller.h"

namespace core {
namespace commands {

using libs::CLI::Command;

class SetTemperature : public Command {
 public:
    SetTemperature() : Command("temp", 1) {
    }

    void callback(const libs::array_view<char *> parameters) override {
        controller.pid.target = std::atof(parameters[0]);
    }
} setTemperature;

class SetPIDCoefficients : public Command {
 public:
    SetPIDCoefficients() : Command("pid", 3) {
    }

    void callback(const libs::array_view<char *> parameters) override {
        controller.pid.Kp = std::atof(parameters[0]);
        controller.pid.Ki = std::atof(parameters[1]);
        controller.pid.Kd = std::atof(parameters[2]);

        controller.pid.reset();
    }
} setPIDCoefficients;

};  // namespace commands
};  // namespace core
