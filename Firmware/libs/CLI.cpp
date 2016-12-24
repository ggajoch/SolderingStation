#include <array>
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include "CLI.h"

namespace libs {
namespace CLI {

libs::array_view<Command*> commands;

void set_commands(libs::array_view<Command*> cmds) {
    commands = cmds;
}

bool parse_line(char* line) {
    //    printf("parse: %s\n", line);
    size_t len = strlen(line);
    static std::array<char*, 20> params;

    for (auto i = 0; i < len; ++i) {
        if (std::isspace(line[i])) {
            line[i] = '\0';
        }
    }

    for (auto cmd : commands) {
        //        printf("now: %s\n", cmd->name);
        if (strcmp(cmd->name, line) == 0) {
            // found command
            //            printf("cmd: %s\n", cmd->name);

            char* iter = line;
            uint8_t param_nr = 0;

            while (iter < line + len) {
                while (iter < line + len && *iter != '\0')
                    iter++;

                while (iter < line + len && *iter == '\0')
                    iter++;

                if (iter < line + len) {
                    params[param_nr++] = iter;
                    //                    printf("param: |%s|\n", iter);
                }
            }

            libs::array_view<char*> view(params.data(), param_nr);
            return cmd->callbackDispatcher(view);
        }
    }
    // printf("No such command: |%s|!\r\n", line);
    return false;
}

}  // namespace CLI
}  // namespace libs
