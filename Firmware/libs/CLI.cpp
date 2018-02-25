#include <array>
#include <cstring>
#include <gsl/span>

#include "CLI.h"

namespace libs::CLI {

gsl::span<Command*> commands;

void set_commands(gsl::span<Command*> cmds) {
    commands = cmds;
}

bool parse_line(char* line) {
    size_t len = strlen(line);
    static std::array<char*, 20> params;

    for (size_t i = 0; i < len; ++i) {
        if (std::isspace(line[i])) {
            line[i] = '\0';
        }
    }

    for (auto cmd : commands) {
        if (strcmp(cmd->name, line) == 0) {
            // found command
    
            char* iter = line;
            uint8_t param_nr = 0;

            while (iter < line + len) {
                while (iter < line + len && *iter != '\0')
                    iter++;

                while (iter < line + len && *iter == '\0')
                    iter++;

                if (iter < line + len) {
                    params[param_nr++] = iter;
                }
            }

            gsl::span<char*> view(params.data(), param_nr);
            return cmd->callbackDispatcher(view);
        }
    }
    return false;
}

}  // namespace libs::CLI
