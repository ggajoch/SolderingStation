#include "CLI.h"

namespace libs {
namespace CLI {

size_t nrOfCommands = 0;
std::array<Command *, 50> commands;

void parse_line(char *line) {
//    printf("parse: %s\n", line);
    size_t len = strlen(line);
    static std::array<char*, 20> params;

    for (auto i = 0; i < len; ++i) {
        if (isspace(line[i])) {
            line[i] = '\0';
        }
    }



    for (int i = 0; i < nrOfCommands; ++i) {
        auto cmd = commands[i];
//        printf("now: %s\n", cmd->name);
        if (strcmp(cmd->name, line) == 0) {
            // found command
//            printf("cmd: %s\n", cmd->name);

            char * iter = line;
            uint8_t param_nr = 0;

            while (iter < line+len) {
                while (iter < line+len && *iter != '\0')
                    iter++;

                while (iter < line+len && *iter == '\0')
                    iter++;

                if (iter < line+len) {
                    params[param_nr++] = iter;
//                    printf("param: |%s|\n", iter);
                }
            }

            libs::array_view<char *> view(params.data(), param_nr);
            cmd->callbackDispatcher(view);
            return;
        }
    }
    printf("No such command: |%s|!\r\n", line);
}

};  // namespace CLI
};  // namespace libs
