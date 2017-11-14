#include <chrono>
#include <cstring>
#include <thread>
#include <iostream>
#include <sstream>

#include "core.h"
#include "HALsim.h"
#include "com.h"
#include "plantModel.h"

#include <ncurses.h>

#include "socket.h"

using namespace std::chrono_literals;

TipModel model;

WINDOW *window;


void parse_sim_command(std::string command) {
    std::string cmd;

    std::istringstream is(command);

    is >> cmd;

    if (cmd == "temp") {
    } else if (cmd == "stand") {
        is >> HAL::Tip::inStandFlag;
    } else if (cmd == "button") {
        HAL::Encoder::buttonHandler();
    }
}

void tick() {
    while(true) {
        core::tick();

        if (HAL::Tip::inStandFlag) {
            attron(COLOR_PAIR(2));
        }
        mvprintw(0, 55, "STAND");
        attron(COLOR_PAIR(1));

        mvprintw(5, 20, "PID target: %.2f     ", core::pid.target);
        refresh();

        model.tick(HAL::Tip::heatingPercentage);

        std::this_thread::sleep_for(100ms);
    }
}

void handle_mouse(int x, int y) {
    if (y == 0) {
        if (x >= 25 && x < 30) {
            // LEFT
            HAL::Encoder::count--;
        }
        if (x >= 35 && x < 40) {
            // RIGHT
            HAL::Encoder::count++;
        }
        if (x >= 45 && x < 53) {
            // BUTTON
            HAL::Encoder::buttonHandler();
        }
        if (x >= 55 && x < 63) {
            // STAND
            HAL::Tip::inStandFlag = !HAL::Tip::inStandFlag;
        }
    }
}

void handle_serial(const char * cmd) {
    static char data[100];
    strcpy(data, cmd);
    HAL::Com::handler(data);
}

int main() {
    initscr();
    start_color();
    window = newwin(100, 100, 6, 0);
    scrollok(window, TRUE);
    noecho();
    raw();
    keypad(stdscr, TRUE);
    cbreak();

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);

    attron(COLOR_PAIR(1));

    mousemask(ALL_MOUSE_EVENTS, NULL);
    MEVENT event;
    refresh();


    mvprintw(0, 25, "LEFT");
    mvprintw(0, 35, "RIGHT");
    mvprintw(0, 45, "BUTTON");
    mvprintw(0, 55, "STAND");

    core::setup();

    handle_serial("pid 1 1 0");
    core::tick();
    handle_serial("tip 0 0.1");
    core::tick();
    handle_serial("disp 0 0");
    core::tick();

    std::thread tick_thread(tick);
    std::thread network_thread(::network_thread);

    while (true) {
        int ch = getch();

        if(ch == KEY_RIGHT) {
            HAL::Encoder::count++;
        } else if(ch == KEY_LEFT) {
            HAL::Encoder::count--;
        } else if(ch == KEY_DOWN) {
            HAL::Encoder::buttonHandler();
        } else if(ch == 'r') {
            core::setup();
        } else if(ch == KEY_MOUSE) {
            if(getmouse(&event) == OK && event.bstate & BUTTON1_CLICKED) {
                handle_mouse(event.x, event.y);
            }
        } else {
            char tmp[100];
            getstr(tmp);

            std::string line(tmp);

            std::string command, params;

            std::istringstream is(line);

            is >> command;
            std::string _;
            std::getline(is, _, ' ');

            std::getline(is, params);

            if (command == "serial" or command == "s") {
                handle_serial(params.c_str());
            } else if (command == "tick" or command == "t") {
                core::tick();
            } else if (command == "sim") {
                parse_sim_command(params);
            }
        }
    }
}
