#include <cstdlib>

#include "CLI.h"
#include "com.h"
#include "core.h"
#include "display.h"
#include "stateManager.h"

namespace core {
namespace commands {

using libs::CLI::Command;

template<typename T,
         std::enable_if_t<std::is_floating_point<T>::value>* = nullptr>
auto parse(const char* parameter) {
    return static_cast<T>(std::atof(parameter));;
}

template<typename T,
         std::enable_if_t<std::is_integral<T>::value>* = nullptr>
auto parse(const char* parameter) {
    return static_cast<T>(std::atoi(parameter));
}

#define WRITE(nr, param) (param) = parse<decltype(param)>(parameters[(nr)])

class SetTemperature : public Command {
 public:
    SetTemperature() : Command("temp", 1) {
    }

    void callback(const gsl::span<char*> parameters) override {
        WRITE(0, core::persistent_state.target);
    }
};

class SetPIDCoefficients : public Command {
 public:
    SetPIDCoefficients() : Command("pid", 4) {
    }

    void callback(const gsl::span<char*> parameters) override {
        WRITE(0, settings.pid.Kp);
        WRITE(1, settings.pid.Ki);
        WRITE(2, settings.pid.Kd);
        WRITE(3, settings.pid.max_power);

        core::pid.reset();
        core::stateManager::config_command_received(core::stateManager::Command::Pid);
    }
};

class SetTipScaling : public Command {
 public:
    SetTipScaling() : Command("tip", 3) {
    }

    void callback(const gsl::span<char*> parameters) override {
        WRITE(0, settings.tip.offset);
        WRITE(1, settings.tip.gain);
        WRITE(2, settings.tip.max_safe_temperature);

        core::stateManager::config_command_received(core::stateManager::Command::Tip);
    }
};

class SendConfig : public Command {
 public:
    SendConfig() : Command("conf", 0) {
    }

    void callback(const gsl::span<char*>) override {
        core::com::printf("conf %d %.2f %.2f %.2f %u %.2f %.2f %u %d %d %u %u %u %u\n",
            core::persistent_state.target,
            settings.pid.Kp,
            settings.pid.Ki,
            settings.pid.Kd,
            settings.pid.max_power,
            settings.tip.offset,
            settings.tip.gain,
            settings.tip.max_safe_temperature,
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
        uint8_t backlight, contrast;
        WRITE(0, backlight);
        WRITE(1, contrast);

        core::display::setDisplaySettings(backlight, contrast);
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

#undef WRITE

}  // namespace commands
}  // namespace core
