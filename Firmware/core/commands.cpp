#include <cstdlib>

#include "CLI.h"
#include "com.h"
#include "core.h"
#include "display.h"
#include "stateManager.h"
#include "storage/persistent_state.h"

namespace core {
namespace commands {

using libs::CLI::Command;

class SetTemperature : public Command {
 public:
    SetTemperature() : Command("temp", 1) {
    }

    void callback(const gsl::span<char*> parameters) override {
        core::persistent_state.target = static_cast<uint16_t>(std::atoi(parameters[0]));
        core::com::printf("temp %d\n", core::persistent_state.target);
    }
};

class SetPIDCoefficients : public Command {
 public:
    SetPIDCoefficients() : Command("pid", 4) {
    }

    void callback(const gsl::span<char*> parameters) override {
        settings.pidParams.Kp = static_cast<float>(std::atof(parameters[0]));
        settings.pidParams.Ki = static_cast<float>(std::atof(parameters[1]));
        settings.pidParams.Kd = static_cast<float>(std::atof(parameters[2]));
        settings.pidParams.max_power = static_cast<float>(std::atof(parameters[3]));
        core::pid.reset();
        core::stateManager::config_command_received(core::stateManager::Command::Pid);
    }
};

class SetTipScaling : public Command {
 public:
    SetTipScaling() : Command("tip", 2) {
    }

    void callback(const gsl::span<char*> parameters) override {
        settings.tipParams.offset = static_cast<float>(std::atof(parameters[0]));
        settings.tipParams.gain = static_cast<float>(std::atof(parameters[1]));
        core::stateManager::config_command_received(core::stateManager::Command::Tip);
    }
};

class SendConfig : public Command {
 public:
    SendConfig() : Command("conf", 0) {
    }

    void callback(const gsl::span<char*>) override {
        core::com::printf("conf %d %.2f %.2f %.2f %u %.2f %.2f %.2f %.2f %u %u %u %u\n",
            core::persistent_state.target,
            settings.pidParams.Kp,
            settings.pidParams.Ki,
            settings.pidParams.Kd,
            settings.pidParams.max_power,
            settings.tipParams.offset,
            settings.tipParams.gain,
            core::settings.display.backlight,
            core::settings.display.contrast,
            core::settings.sleep_temperature,
            core::settings.stand_temperature,
            core::settings.timeouts.sleep,
            core::settings.timeouts.off);
    }
};

class Display : public Command {
 public:
    Display() : Command("disp", 2) {
    }

    void callback(const gsl::span<char*> parameters) override {
        core::display::setDisplaySettings(static_cast<float>(std::atof(parameters[0])), static_cast<float>(std::atof(parameters[1])));
        core::stateManager::config_command_received(core::stateManager::Command::Display);
    }
};

class StandbyTemperatures : public Command {
 public:
    StandbyTemperatures() : Command("stdby", 2) {
    }

    void callback(const gsl::span<char*> parameters) override {
        core::settings.sleep_temperature = static_cast<uint16_t>(std::atoi(parameters[0]));
        core::settings.stand_temperature = static_cast<uint16_t>(std::atoi(parameters[1]));
        core::stateManager::config_command_received(core::stateManager::Command::StandbyTemperatures);
    }
};

class SetTimeouts : public Command {
 public:
    SetTimeouts() : Command("timeouts", 2) {
    }

    void callback(const gsl::span<char*> parameters) override {
        core::settings.timeouts.sleep = static_cast<uint8_t>(std::atoi(parameters[0]));
        core::settings.timeouts.off = static_cast<uint8_t>(std::atoi(parameters[1]));
        core::stateManager::config_command_received(core::stateManager::Command::Timeouts);
    }
};

class Ping : public Command {
 public:
    Ping() : Command("ping", 0) {
    }

    void callback(const gsl::span<char*>) override {
        core::com::printf("pong\n");
    }
};

void setup() {
    static SetTemperature setTemperature;
    static SetPIDCoefficients setPIDCoefficients;
    static SetTipScaling setTipScaling;
    static SendConfig sendConfig;
    static Display display;
    static StandbyTemperatures standbyTemperatures;
    static SetTimeouts setTimeouts;
    static Ping ping;

    static Command* commands[] = {
        &setTemperature, &setPIDCoefficients, &setTipScaling, &sendConfig, &standbyTemperatures, &setTimeouts, &ping, &display};
    libs::CLI::set_commands(gsl::span<Command*>(commands));
}

}  // namespace commands
}  // namespace core
