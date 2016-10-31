#include <chrono>
#include <cstring>
#include "gtest/gtest.h"

#include "HALsim.h"
#include "plantModel.h"
#include "CLI.h"
#include "core.h"

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


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    model.soldering(false);
    HAL::Tip::setTemperature(20);

    core::setup();


    while (true) {
        core::tick();

        // simulation-events
        simTick();
    }

    return RUN_ALL_TESTS();
}
