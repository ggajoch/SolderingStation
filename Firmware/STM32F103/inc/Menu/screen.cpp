#include <stdio.h>

#include "screen.h"
#include "weller/weller.h"
#include "LCD/LCD_trans.h"


Main::Main() {
	stand_by = false;
	off = true;
	solderingIron.pid.setTarget(0);
}

void Main::display() {
	LCDScreen out;
	static char buf[17];

	sprintf(buf, "%.1f/%.1f", solderingIron.filtered_temp, solderingIron.getTemp());

	sprintf(out.top, "%*s", 16, buf);

	sprintf(buf, "power: %.1f", solderingIron.display_power);
	sprintf(out.down, "%*s", 16, buf);

	if( solderingIron.state == Weller::off ) {
		sprintf(out.down, "OFF  ");
	} else if( solderingIron.state == Weller::onStand ) {
		sprintf(out.down, "STAND");
	} else {
		sprintf(out.down, "HEAT ");
	}
	out.down[5] = ' ';

	LCD_PutText(0, 0, out.top);
	LCD_PutText(0, 1, out.down);
}
void Main::plus() {
	if( solderingIron.state != Weller::heat )
		return;
	float now = solderingIron.getTemp();
	now += 2.0;
	solderingIron.setTemp(now);
}

void Main::minus() {
	if( solderingIron.state != Weller::heat )
			return;
	float now = solderingIron.getTemp();
	now -= 2.0;
	solderingIron.setTemp(now);
}

static float normalTemp = 0;

void Main::click() {
	if( solderingIron.state == Weller::off ||
		solderingIron.state == Weller::standby ) {
		solderingIron.state = Weller::heat;
	}
	else { // heat
		solderingIron.state = Weller::off;
	}
}
void Main::long_click() {
}
