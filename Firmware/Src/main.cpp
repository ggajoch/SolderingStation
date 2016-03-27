#include <stdint.h>
#include "HAL.h"

int main(void) {
    WellerHAL hal;
    float val = 0;
    while (1) {
        printf("test %f\r\n", val);
		hal.setBacklight(val);
		val += 1;
		if( val >= 100 ) {
			val = 0;
		}
		HAL_Delay(10);
    }
}
