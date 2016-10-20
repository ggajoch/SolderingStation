#ifndef SOLDER_WELLER_H
#define SOLDER_WELLER_H

#include "Tip.h"
#include "AVG.h"
#include "pid.h"

class WellerController_t {
    constexpr static int TEMP_AVERAGE = 10;

public:
    void tick() {
        HAL::setHeating(0);
        HAL_Delay(15);
        for(uint8_t i = 0; i < TEMP_AVERAGE; ++i) {
            average.put(thermocouple.getTemperature());
        }
        float temp = average.get();
        float ctrl = tempPID.tick(temp);

        static char buf[16] = {' '};
        sprintf(buf, "temp: %.1f  ", temp);
        HD44780::PutText(0, 0, buf);

        sprintf(buf, "pwr: %.1f   ", ctrl);
        HD44780::PutText(0, 1, buf);
        printf("tick %f %f\r\n", temp, ctrl);

        HAL::setHeating(ctrl);
    }


    void setTemperature(float temp) {
        this->tempPID.setTarget(temp);
    }

    PID tempPID;

private:
    Thermocouple_t thermocouple;
    Average<float, TEMP_AVERAGE> average;
} WellerController;

#endif //SOLDER_WELLER_H
