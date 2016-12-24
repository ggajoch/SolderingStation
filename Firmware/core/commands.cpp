#include <cstdlib>

#include "CLI.h"
#include "core.h"
#include "com.h"
#include "display.h"

namespace core {
namespace commands {

using libs::CLI::Command;

class SetTemperature : public Command {
 public:
    SetTemperature() : Command("temp", 1) {
    }

    void callback(const libs::array_view<char *> parameters) override {
        core::target = static_cast<float>(std::atof(parameters[0]));
        core::com::printf("temp %f\n", core::pid.target);
    }
};

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
};

class SetTipScaling : public Command {
 public:
    SetTipScaling() : Command("tip", 2) {
    }

    void callback(const libs::array_view<char *> parameters) override {
        core::tempSensor::params.offset = static_cast<float>(std::atof(parameters[0]));
        core::tempSensor::params.gain = static_cast<float>(std::atof(parameters[1]));
    }
};


class SendConfig : public Command {
 public:
    SendConfig() : Command("conf", 0) {
    }

    void callback(const libs::array_view<char *> parameters) override {
        core::com::printf("conf %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f\n",
                          core::target,
                          core::pid.params.Kp,
                          core::pid.params.Ki,
                          core::pid.params.Kd,
                          core::tempSensor::params.offset,
                          core::tempSensor::params.gain,
                          core::display::backlight,
                          core::display::contrast);
    }
};

class Display : public Command {
 public:
    Display() : Command("disp", 2) {
    }

    void callback(const libs::array_view<char *> parameters) override {
        core::display::setBacklight(static_cast<float>(std::atof(parameters[0])));
        core::display::setContrast(static_cast<float>(std::atof(parameters[1])));
    }
};

class Ping : public Command {
 public:
    Ping() : Command("ping", 0) {
    }

    void callback(const libs::array_view<char *> parameters) override {
        core::com::printf("ping\n");
        core::com::printf("ping\n");
    }
};


void setup() {
	static SetTemperature setTemperature;
	static SetPIDCoefficients setPIDCoefficients;
	static SetTipScaling setTipScaling;
	static SendConfig sendConfig;
	static Display display;
	static Ping ping;
	
	static Command * commands[] = {
		&setTemperature,
		&setPIDCoefficients,
		&setTipScaling,
		&sendConfig,
		&ping,
		&display

	};
	libs::CLI::set_commands(libs::array_view<Command *>(commands));
}

};  // namespace commands
};  // namespace core
