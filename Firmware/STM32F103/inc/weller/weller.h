#ifndef WELLER_H_
#define WELLER_H_

#include <stdint.h>
#include "thermocouple.h"
#include "pid.h"
#include "utils/AVG.h"

class Weller {

public:
	float last_temp, filtered_temp,
	      power, filtered_power, display_power;

	Thermocouple temp;
	PID pid;
	AVG<float, 10> average;
	AVG<float, 2> power_avg;
	AVG<float, 10> power_display_avg;

	enum Mode {
		heat,
		standby,
		onStand,
		off
	};
	Mode state;
	Weller() {
		state = off;
	}
	void tick(uint16_t adc) {
		last_temp = temp.getTemp(adc);
		average.put(last_temp);
		filtered_temp = average.get();

		if( state == off || state == onStand ) {
			power = 0;
			pid.reset();
		} else {
			power = pid.tick(filtered_temp);
		}

		power_avg.put(power);
		filtered_power = power_avg.get();

		power_display_avg.put(filtered_power);
		display_power = power_display_avg.get();
	}

	void setTemp(float temp) {
		pid.setTarget(temp);
	}

	float getTemp() {
		if( state == off || state == onStand ) {
			return 0;
		} else {
			return pid.getTarget();
		}
	}
};

extern Weller solderingIron;


#endif // WELLER_H_
