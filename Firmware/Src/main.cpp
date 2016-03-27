#include <stdint.h>
#include <array>
#include <stm32f103xb.h>
#include <HD44780.h>
#include "HAL.h"
#include "CLI.h"
#include "LCD_trans.h"

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

    hal->setContrast(30);

	Command cmd_x = {"abc", first};
    Command cmd_y = {"back", back};
    Command cmd_contr = {"contr", contr};
    Command arr[] = {cmd_x, cmd_y, cmd_contr};
    CLI cliInst(sizeof(arr)/sizeof(Command), arr, write);
    cli = & cliInst;

    HD44780::Init();
    HD44780::PutText(0, 0, "abcdefg");


    HAL_Delay(1000);

    HD44780::PutText(0, 1, "defxyz");
    for(int i = 0; i < 100; ++i)
        HD44780::TimeTick();
    HAL_Delay(1000);
    HD44780::Clear();
    for(int i = 0; i < 100; ++i)
        HD44780::TimeTick();
    HAL_Delay(1000);
    HD44780::PutText(1, 1, "poiuytrewq");

    while(1) {
        HD44780::TimeTick();
    }
    while (1) {
		val += 1;
		if( val >= 100 ) {
			val = 0;
		}
		HAL_Delay(10);
    }
}
