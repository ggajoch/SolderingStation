#include <chrono>
#include <cstring>
#include <thread>
#include <iostream>

#include "core.h"
#include "HALsim.h"
#include "com.h"

void input() {
    while(true) {
        char data[100];
        std::cin.getline(data, 100);
        HAL::Com::callback(data);
    }
}

auto tick_duration = std::chrono::milliseconds{100};

void loop() {
    core::setup();

    std::chrono::system_clock::time_point next_point = std::chrono::system_clock::now();

    while (true) {
        std::this_thread::sleep_until(next_point);
        next_point += tick_duration;

        core::tick();
    }
}

int main() {
    std::thread input_thread(input);
    std::thread loop_thread(loop);

    input_thread.join();
    loop_thread.join();
}
