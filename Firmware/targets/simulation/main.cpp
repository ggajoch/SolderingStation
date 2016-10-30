#include <chrono>
#include <cstring>
#include <thread>

#include "gtest/gtest.h"

#include "Controller.h"
#include "HALsim.h"
#include "plantModel.h"
#include "CLI.h"
#include "core.h"

#include "Serial.h"
using core::controller;
TipModel model;

void sendCommand(const char * data) {
    static char buf[100];
    std::strcpy(buf, data);
    HAL::Com::callback(buf);
}

void simTick() {
    for (int i = 0; i < 10; ++i) {
        model.tick(HAL::Tip::heatingPercentage);
        HAL::Tip::setTemperature(model.Ttip);
        core::com::printf("TICK %.2f %.2f %.2f %.2f\n",
                          model.Ttip,
                          controller.pid.target,
                          controller.lastPower,
                          controller.pid.integral);
    }
}

class JointCmd : libs::CLI::Command {
 public:
    JointCmd() : Command("joint", 1) {}

 private:
    void callback(const libs::array_view<char *> parameters) override {
        if (parameters[0][0] == '1') {
            model.soldering(true);
        } else {
            model.soldering(false);
        }
    }
} jointCmd;

Serial * serial;

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    model.soldering(false);
    HAL::Tip::setTemperature(20);

    core::setup();
    core::controller.tip.gain = 0.1;

    serial = new Serial("COM1");

    printf("Connect: %d\n", serial->IsConnected());


    std::string buffer;
    std::chrono::system_clock::time_point next_point = std::chrono::system_clock::now();

    next_point += std::chrono::milliseconds{100};

    while (true) {
        std::this_thread::sleep_until(next_point);
        next_point += std::chrono::milliseconds{100};

        char buf[100];
        int readed = serial->ReadData(buf, 100);
        buf[readed] = '\0';

        std::string now{buf};
        buffer += now;

        std::size_t pos;
        while ((pos = buffer.find('\n')) != std::string::npos) {
            std::string cmd{buffer.substr(0, pos+1)};
            buffer = buffer.substr(pos+1, buffer.size());

            char data[100];
            strcpy(data, cmd.c_str());

            HAL::Com::callback(data);
        }

//        serial.WriteData("test\r", 5);
        core::tick();

        // simulation-events
        simTick();
    }

    return RUN_ALL_TESTS();
}
