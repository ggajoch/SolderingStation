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
        controller.pid.target = static_cast<float>(std::atof(parameters[0]));
        printf("temp %f\n", controller.pid.target);
    }
} setTemperature;

class SetPIDCoefficients : public Command {
 public:
    SetPIDCoefficients() : Command("pid", 3) {
    }

    void callback(const libs::array_view<char *> parameters) override {
        controller.pid.Kp = static_cast<float>(std::atof(parameters[0]));
        controller.pid.Ki = static_cast<float>(std::atof(parameters[1]));
        controller.pid.Kd = static_cast<float>(std::atof(parameters[2]));

        controller.pid.reset();
        printf("pid %f %f %f\n", controller.pid.Kp, controller.pid.Ki, controller.pid.Kd);
    }
} setPIDCoefficients;

class SetTipScaling : public Command {
 public:
    SetTipScaling() : Command("tip", 2) {
    }

    void callback(const libs::array_view<char *> parameters) override {
        controller.tip.offset = static_cast<float>(std::atof(parameters[0]));
        controller.tip.gain = static_cast<float>(std::atof(parameters[1]));
        printf("tip %f %f\n", controller.tip.offset, controller.tip.gain);
    }
} setTipScaling;

};  // namespace commands
};  // namespace core
