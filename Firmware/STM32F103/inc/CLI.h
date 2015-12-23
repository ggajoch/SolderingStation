#ifndef CLI_H_
#define CLI_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct Command {
	char * name;
	void (*callback)(float * params, uint8_t nrOfParams);
};

class CLI {
	static const uint8_t max_commands = 100;
	static const uint8_t max_parameters = 10;
    static const uint8_t max_cmd_len = 100;
	struct CommandObject {
		CommandObject() {}
		CommandObject(Command cmd) : cmd(cmd) {
			len = strlen(cmd.name);
		}
		Command cmd;
		uint8_t len;
		bool operator==(const CommandObject & rhs) const {
			if( strcmp(this->cmd.name, rhs.cmd.name) == 0 ) {
				return true;
			} else {
				return false;
			}
		}
	};
	CommandObject command_list[max_commands];

	uint8_t registered_commands;
    void (*writer)(const char *);
public:
    CLI(void (*writer)(const char *)) : registered_commands(0), writer(writer) {

	}

	void registerCommand(Command cmd) {
		CommandObject cmdx(cmd);
		command_list[registered_commands] = cmdx;
		registered_commands++;
	}

	void parse_line(char * line) {
		uint8_t len = strlen(line);
		uint8_t iter = 0;
		float params[max_parameters];
		while(line[iter] != ' ' && line[iter] != 0) ++iter;
		line[iter] = 0;

		for(uint8_t cmd_nr = 0; cmd_nr < this->registered_commands; ++cmd_nr) {
			if( strcmp(this->command_list[cmd_nr].cmd.name, line) == 0 ) {
				// found command
				uint8_t param_nr = 0;
				while(iter < len) {
					iter++;
					char * stop;
					float val = strtof(line+iter, &stop);
					params[param_nr++] = val;
					iter = stop-line;
				}
				command_list[cmd_nr].cmd.callback(params, param_nr);
			}
		}
	}

    void sendCommand(const char * command, float * params, uint8_t params_nr) {
        char buf[max_cmd_len];
        uint8_t iter = 0;
        strcpy(buf, command);
        iter += strlen(command);
        while( params_nr-- ) {
            buf[iter++] = ' ';
            iter += sprintf(buf+iter, "%f", *params);
            params++;
        }
        buf[iter++] = 0;
        this->writer(buf);
    }
};


#endif // CLI_H_
