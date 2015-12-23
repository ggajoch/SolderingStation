#ifndef WELLER_H_
#define WELLER_H_

#include "thermocouple.h"
#include "pid.h"
#include "utils/AVG.h"

class Weller {

public:
	float last_temp, filtered_temp, power;

	Thermocouple temp;
	PID pid;
	AVG<float, 10> average;
	void tick(uint16_t adc) {
		last_temp = temp.getTemp(adc);
		average.put(last_temp);
		filtered_temp = average.get();

		power = pid.tick(filtered_temp);
	}
};

Weller solderingIron;


#endif // WELLER_H_
