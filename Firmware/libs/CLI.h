#ifndef LIBS_CLI_H_
#define LIBS_CLI_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <array>
#include "array_view.h"

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
    }

    virtual void callback(const array_view<char *> parameters) {
    }

    const char * name;
};

void parse_line(char *line);

};  // namespace CLI
};  // namespace libs

#endif  // LIBS_CLI_H_
