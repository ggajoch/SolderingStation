#include <cstdio>

#include "CLI.h"
#include "core.h"
#include "com.h"

namespace core {
namespace commands {

using libs::CLI::Command;

class SetTemperature : public Command {
 public:
    SetTemperature() : Command("temp", 1) {
    }

    void callback(const libs::array_view<char *> parameters) override {
        core::target = static_cast<float>(std::atof(parameters[0]));
        printf("temp %f\n", core::pid.target);
    }
} setTemperature;

class SetPIDCoefficients : public Command {
 public:
    SetPIDCoefficients() : Command("pid", 3) {
    }

    void callback(const libs::array_view<char *> parameters) override {
        core::pid.params.Kp = static_cast<float>(std::atof(parameters[0]));
        core::pid.params.Ki = static_cast<float>(std::atof(parameters[1]));
        core::pid.params.Kd = static_cast<float>(std::atof(parameters[2]));
        core::pid.reset();
    }
} setPIDCoefficients;

class SetTipScaling : public Command {
 public:
    SetTipScaling() : Command("tip", 2) {
    }

    void callback(const libs::array_view<char *> parameters) override {
        core::tip.params.offset = static_cast<float>(std::atof(parameters[0]));
        core::tip.params.gain = static_cast<float>(std::atof(parameters[1]));
    }
} setTipScaling;


class SendConfig : public Command {
 public:
    SendConfig() : Command("conf", 0) {
    }

    void callback(const libs::array_view<char *> parameters) override {
        core::com::printf("conf %.2f %.2f %.2f %.2f %.2f %.2f\n",
                          core::target,
                          core::pid.params.Kp,
                          core::pid.params.Ki,
                          core::pid.params.Kd,
                          core::tip.params.offset,
                          core::tip.params.gain);
    }
} sendConfig;

class Ping : public Command {
 public:
    Ping() : Command("ping", 0) {
    }

    void callback(const libs::array_view<char *> parameters) override {
        core::com::printf("ping\n");
        std::printf("ping\n");
    }
} ping;

};  // namespace commands
};  // namespace core
