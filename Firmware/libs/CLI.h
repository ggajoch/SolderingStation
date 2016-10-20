#ifndef CLI_H_
#define CLI_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "ArrayView.h"

namespace CLI {
    class Command;

    size_t nrOfCommands = 0;
    std::array<Command *, 50> commands;

    class Command {
    public:
        Command(const char *name) : name(name) {
            commands[nrOfCommands++] = this;
        }

        virtual void callback(const arv::array_view<float> & parameters) {}

        const char *name;

    private:

        bool operator==(const Command &rhs) const {
            if (strcmp(this->name, rhs.name) == 0) {
                return true;
            } else {
                return false;
            }
        }
    };


    void parse_line(const char *line) {
        size_t len = strlen(line);
        uint8_t iter = 0;
        float params[10];
        while (line[iter] != ' ' && line[iter] != 0) ++iter;

        for( auto &cmd : commands ) {
            if (strncmp(cmd->name, line, iter - 1) == 0) {
                // found command
                uint8_t param_nr = 0;
                while (iter < len) {
                    iter++;
                    char *stop;
                    float val = strtof(line + iter, &stop);
                    params[param_nr++] = val;
                    iter = stop - line;
                }
                arv::array_view<float> view(params, param_nr);
                cmd->callback(view);
            }
        }
    }

    void sendCommand(const Command & command, const arv::array_view<float> & params) {
        static char buf[100];
        uint8_t iter = 0;
        strcpy(buf, command.name);
        iter += strlen(command.name);
        for(auto &param : params) {
            buf[iter++] = ' ';
            iter += sprintf(buf + iter, "%f", param);
        }
        buf[iter] = 0;
        printf("%s\r\n", buf);
    }
}
#endif // CLI_H_
