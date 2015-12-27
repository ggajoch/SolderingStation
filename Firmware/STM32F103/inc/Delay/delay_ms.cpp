#include <stdint.h>
#include "delay_ms.h"

volatile uint32_t delay_1 = 0;

void _delay_ms(uint32_t delay){
	delay_1 = delay*10;
	while( delay_1 != 0 );
}

void DelayDecrement(void){
	if( delay_1 > 0 ) delay_1--;
}
