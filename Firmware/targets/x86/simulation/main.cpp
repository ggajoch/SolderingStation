#include <chrono>
#include <cstring>
#include <thread>
#include <iostream>
#include <sstream>

#include "core.h"
#include "HALsim.h"
#include "com.h"

using namespace std::chrono_literals;

void parse_sim_command(std::string command) {
    std::string cmd;

    std::istringstream is(command);

    is >> cmd;

    if (cmd == "temp") {
        is >> HAL::Tip::temperature;
    } else if (cmd == "stand") {
        is >> HAL::Tip::inStandFlag;
    } else if (cmd == "button") {
        HAL::Encoder::buttonHandler();
    }
}

void tick() {
    while(true) {
        core::tick();
        std::this_thread::sleep_for(100ms);
    }
}

int main() {
    core::setup();

    std::thread tick_thread(tick);

    while (true) {
        std::string line;
        std::getline(std::cin, line);

        std::string command, params;

        std::istringstream is(line);

        is >> command;
        std::string _;
        std::getline(is, _, ' ');

        std::getline(is, params);

//        std::cout << "Cmd:" << command << " with params |" << params << "|" << std::endl;

        if (command == "serial" or command == "s") {
            char data[100];
            strcpy(data, params.c_str());
            HAL::Com::handler(data);
        } else if (command == "tick" or command == "t") {
            core::tick();
        } else if (command == "sim") {
            parse_sim_command(params);
        }
    }
}
