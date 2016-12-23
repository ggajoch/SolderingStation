#ifndef LIBS_CLI_H_
#define LIBS_CLI_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <array>

#include "array_view.h"
// #include "Com.h"
#include <cstdio>
namespace libs {
namespace CLI {

class Command {
 public:
    explicit Command(const char * name) :
            name(name) {
		std::printf("registering command %s\n", name);
        requiredArguments = -1;
    }

    Command(const char * name, int requiredArguments) :
            name(name),
            requiredArguments(requiredArguments) {
    }

    bool callbackDispatcher(const array_view<char *> parameters) {
        if (requiredArguments > -1 && parameters.size() != requiredArguments) {
            return false;
        }
        callback(parameters);
		return true;
    }

    virtual void callback(const libs::array_view<char *> parameters) {}

    const char * name;
    int requiredArguments;
};

void set_commands(libs::array_view<Command *> cmds);
bool parse_line(char *line);

};  // namespace CLI
};  // namespace libs

#endif  // LIBS_CLI_H_
