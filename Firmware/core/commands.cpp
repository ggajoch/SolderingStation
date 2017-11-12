#include <cstdlib>

#include "CLI.h"
#include "com.h"
#include "core.h"
#include "display.h"
#include "storage/persistent_state.h"
#include "sleepManager.h"

namespace core {
namespace commands {

using libs::CLI::Command;

class SetTemperature : public Command {
 public:
    SetTemperature() : Command("temp", 1) {
    }

    void callback(const gsl::span<char*> parameters) override {
        core::persistent_state.target = static_cast<float>(std::atof(parameters[0]));
        core::com::printf("temp %.2f\n", core::persistent_state.target);
    }
};

class SetPIDCoefficients : public Command {
 public:
    SetPIDCoefficients() : Command("pid", 3) {
    }

    void callback(const gsl::span<char*> parameters) override {
        settings.pidParams.Kp = static_cast<float>(std::atof(parameters[0]));
        settings.pidParams.Ki = static_cast<float>(std::atof(parameters[1]));
        settings.pidParams.Kd = static_cast<float>(std::atof(parameters[2]));
        core::pid.reset();
    }
};

class SetTipScaling : public Command {
 public:
    SetTipScaling() : Command("tip", 2) {
    }

    void callback(const gsl::span<char*> parameters) override {
        settings.tipParams.offset = static_cast<float>(std::atof(parameters[0]));
        settings.tipParams.gain = static_cast<float>(std::atof(parameters[1]));
    }
};

class SendConfig : public Command {
 public:
    SendConfig() : Command("conf", 0) {
    }

    void callback(const gsl::span<char*>) override {
        core::com::printf("conf %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f\n",
            core::persistent_state.target,
            settings.pidParams.Kp,
            settings.pidParams.Ki,
            settings.pidParams.Kd,
            settings.tipParams.offset,
            settings.tipParams.gain,
            core::display::backlight,
            core::display::contrast);
    }
};

class Display : public Command {
 public:
    Display() : Command("disp", 2) {
    }

    void callback(const gsl::span<char*> parameters) override {
        core::display::setBacklight(static_cast<float>(std::atof(parameters[0])));
        core::display::setContrast(static_cast<float>(std::atof(parameters[1])));
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

class AcceptConfig : public Command {
 public:
    AcceptConfig() : Command("config_ok", 0) {
    }

    void callback(const gsl::span<char*>) override {
        core::com::printf("Config OK!\n");
        core::sleepManager::configurationCorrectState = true;
    }
};

void setup() {
    static SetTemperature setTemperature;
    static SetPIDCoefficients setPIDCoefficients;
    static SetTipScaling setTipScaling;
    static SendConfig sendConfig;
    static Display display;
    static Ping ping;
    static AcceptConfig acceptConfig;

    static Command* commands[] = {
        &setTemperature,
        &setPIDCoefficients,
        &setTipScaling,
        &sendConfig,
        &ping,
        &display,
        &acceptConfig
    };
    libs::CLI::set_commands(gsl::span<Command*>(commands));
}

}  // namespace commands
}  // namespace core
