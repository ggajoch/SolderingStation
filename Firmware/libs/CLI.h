#pragma once

#include <gsl/span>

namespace libs::CLI {

constexpr int maximumRequiredArguments = 1001;

class Command {
 public:
    explicit Command(const char* name) : name(name) {
        requiredArguments = maximumRequiredArguments;
    }

    Command(const char* name, unsigned int requiredArguments) : name(name), requiredArguments(requiredArguments) {
    }

    bool callbackDispatcher(const gsl::span<char*> parameters) {
        if (requiredArguments < maximumRequiredArguments && (unsigned int)parameters.size() != requiredArguments) {
            return false;
        }
        callback(parameters);
        return true;
    }

    virtual void callback(const gsl::span<char*>) = 0;

    const char* name;
    unsigned int requiredArguments;
};

void set_commands(gsl::span<Command*> cmds);
bool parse_line(char* line);

}  // namespace libs::CLI
