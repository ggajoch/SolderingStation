#include <stdint.h>
#include <array>
#include <stm32f103xb.h>
#include "HAL.h"
#include "CLI.h"

CLI *cli;
WellerHAL *hal;

void first(float * x, uint8_t len) {
	printf("Got abc: %f\r\n", x[0]);
}

void back(float * x, uint8_t len) {
    printf("back: %f\r\n", x[0]);
    hal->setBacklight(x[0]);
}

void write(const char * buf) {
    printf("%s", buf);
}

int main(void) {
    WellerHAL halInst;
    hal = &halInst;
    float val = 0;

	Command cmd_x = {"abc", first};
    Command cmd_y = {"back", back};
    Command arr[] = {cmd_x, cmd_y};
    CLI cliInst(sizeof(arr)/sizeof(Command), arr, write);
    cli = & cliInst;

    while (1) {
		val += 1;
		if( val >= 100 ) {
			val = 0;
		}
		HAL_Delay(10);
    }
}
