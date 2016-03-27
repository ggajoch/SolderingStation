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

void contr(float * x, uint8_t len) {
    printf("contr: %f\r\n", x[0]);
    hal->setContrast(x[0]);
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
    Command cmd_contr = {"contr", contr};
    Command arr[] = {cmd_x, cmd_y, cmd_contr};
    CLI cliInst(sizeof(arr)/sizeof(Command), arr, write);
    cli = & cliInst;

//    while(1) {
//    	printf("abcd\r\n");
//        HAL_Delay(1);
//    }

    LCD_init();
    LCD_PutText(0, 0, "abcd");
    while(1) {
    	LCD_4us_timeproc();
    	HAL_Delay(1);
    }
    while (1) {
		val += 1;
		if( val >= 100 ) {
			val = 0;
		}
		HAL_Delay(10);
    }
}
