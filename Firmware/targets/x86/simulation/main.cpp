#include <chrono>
#include <cstring>
#include <thread>
#include <iostream>
#include <sstream>

#include "core.h"
#include "HALsim.h"
#include "com.h"


void parse_sim_command(std::string command) {
    std::string cmd;

    std::istringstream is(command);

    is >> cmd;

    if (cmd == "temp") {
        is >> HAL::Tip::temperature;
    } else if (cmd == "stand") {
        is >> HAL::Tip::inStandFlag;
    }
}

int main() {
    core::setup();

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

        if (command == "serial") {
            char data[100];
            strcpy(data, params.c_str());
            HAL::Com::callback(data);
        } else if (command == "tick") {
            core::tick();
        } else if (command == "sim") {
            parse_sim_command(params);
        }
    }
}
