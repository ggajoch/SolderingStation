#ifndef CLI_H_
#define CLI_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct Command {
	char * name;
	void (*callback)(float * params, uint8_t nrOfParams);

	bool operator==(const Command & rhs) const {
		if( strcmp(this->name, rhs.name) == 0 ) {
			return true;
		} else {
			return false;
		}
	}
};

class CLI {
    size_t nrOfCommands;
public:
    CLI(size_t nrOfCommands, const Command* commands, void (*writer)(const char *)) : nrOfCommands(nrOfCommands), commands(commands), writer(writer) {

    }
    const Command *commands;

    void (*writer)(const char *);

	void parse_line(const char * line) {
		size_t len = strlen(line);
		uint8_t iter = 0;
		float params[10];
		while(line[iter] != ' ' && line[iter] != 0) ++iter;

		for(uint8_t cmd_nr = 0; cmd_nr < this->nrOfCommands; ++cmd_nr) {
			if( strncmp(this->commands[cmd_nr].name, line, iter-1) == 0 ) {
				// found command
				uint8_t param_nr = 0;
				while(iter < len) {
					iter++;
					char * stop;
					float val = strtof(line+iter, &stop);
					params[param_nr++] = val;
					iter = stop-line;
				}
				commands[cmd_nr].callback(params, param_nr);
			}
		}
	}

    void sendCommand(Command command, float * params, uint8_t params_nr) {
        char buf[100];
        uint8_t iter = 0;
        strcpy(buf, command.name);
        iter += strlen(command.name);
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
