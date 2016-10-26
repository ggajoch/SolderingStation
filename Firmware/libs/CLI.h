#ifndef LIBS_CLI_H_
#define LIBS_CLI_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <array>

#include "array_view.h"
#include "Com.h"

namespace libs {
namespace CLI {

class Command;

extern size_t nrOfCommands;
extern std::array<Command *, 50> commands;

class Command {
 public:
    explicit Command(const char * name) :
            name(name) {
        commands[nrOfCommands++] = this;
        requiredArguments = -1;
    }

    Command(const char * name, int requiredArguments) :
            name(name),
            requiredArguments(requiredArguments) {
        commands[nrOfCommands++] = this;
    }

    void callbackDispatcher(const array_view<char *> parameters) {
        if (requiredArguments > -1 && parameters.size() != requiredArguments) {
            HAL::Com::printf("ERR Required %d arguments for %s\n", requiredArguments, this->name);
            return;
        }
        callback(parameters);
    }

    virtual void callback(const array_view<char *> parameters) = 0;

    const char * name;
    int requiredArguments;
};

void parse_line(char *line);

};  // namespace CLI
};  // namespace libs

#endif  // LIBS_CLI_H_
