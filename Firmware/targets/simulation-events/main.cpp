#include <chrono>
#include <cstring>
#include <experimental/optional>
#include "gtest/gtest.h"

#include "Controller.h"
#include "HALsim.h"
#include "plantModel.h"
#include "CLI.h"

using core::controller;
TipModel model;

std::chrono::milliseconds tickTime{100};


void sendCommand(const char * data) {
    static char buf[100];
    std::strcpy(buf, data);
    HAL::Com::callback(buf);
}

namespace events {
struct Event {
    void (*function)();
    std::chrono::milliseconds time;

    Event(void (*foo)(), int t) : function(foo), time(t) {}
};

void time0() {
    printf("time 0\n");
    sendCommand("tip 0 0.1\n");
}

void time1000() {
    printf("time 1000\n");
    sendCommand("pid 1 1 0\n");
}

void time2000() {
    printf("time 2000\n");
    sendCommand("temp 300\n");
}

void time10000() {
    printf("SOLDERING\n");
    model.soldering(true);
}

void time30000() {
    printf("AIR\n");
    model.soldering(false);
}

void time50000() {
    std::exit(RUN_ALL_TESTS());
}

#define EVENT(t) {&time##t, t}

Event events[] = {
        EVENT(0),
        EVENT(1000),
        EVENT(2000),
        EVENT(10000),
        EVENT(30000),
        EVENT(50000)
};

int ticksSoFar = 0;
void tick() {
    for (auto &x : events::events) {
        if (x.time == ticksSoFar * tickTime) {
            x.function();
        }
    }
    ticksSoFar++;
}
};  // namespace events

void simTick() {
    model.tick(HAL::Tip::heatingPercentage);
    HAL::Tip::setTemperature(model.Ttip);

    events::tick();
}

std::experimental::optional<char *> rxCommandPtr = {};
void rxCommand(char * data) {
    rxCommandPtr = data;
    printf("RX: |%s|\n", rxCommandPtr);
}

void dispatchCommand() {
    if (rxCommandPtr) {
        printf("dispatch: |%s|\n", *rxCommandPtr);
        libs::CLI::parse_line(*rxCommandPtr);
        rxCommandPtr = {};
    }
    HAL::Com::puts("*");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    model.soldering(false);
    HAL::Tip::setTemperature(20);

    controller.setup();
    HAL::Com::setCallback(rxCommand);
//            libs::CLI::parse_line);

    while (true) {
        if (HAL::timeTick()) {
            controller.loop();
            dispatchCommand();

            // simulation-events
            simTick();
        }
    }

    return RUN_ALL_TESTS();
}
