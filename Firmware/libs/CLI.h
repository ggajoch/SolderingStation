#ifndef LIBS_CLI_H_
#define LIBS_CLI_H_

#include "array_view.h"

namespace libs {
namespace CLI {

constexpr int maximumRequiredArguments = 1001;

class Command {
 public:
    explicit Command(const char* name) : name(name) {
        requiredArguments = maximumRequiredArguments;
    }

    Command(const char* name, unsigned int requiredArguments) : name(name), requiredArguments(requiredArguments) {
    }

    bool callbackDispatcher(const array_view<char*> parameters) {
        if (requiredArguments < maximumRequiredArguments && parameters.size() != requiredArguments) {
            return false;
        }
        callback(parameters);
        return true;
    }

    virtual void callback(const libs::array_view<char*> parameters) {
        UNREFERENCED_PARAMETER(parameters);
    }

    const char* name;
    unsigned int requiredArguments;
};

void set_commands(libs::array_view<Command*> cmds);
bool parse_line(char* line);

}  // namespace CLI
}  // namespace libs

#endif  // LIBS_CLI_H_
