#ifndef SOLDER_WELLER_H
#define SOLDER_WELLER_H

#include "Thermocuple.h"

class WellerController_t {
public:
    void tick() {
        float x = thermocouple.getTemperature();
        char buf[16] = {' '};
        sprintf(buf, "temp: %f    ", x);
        HD44780::PutText(0, 0, buf);
        printf("tick\r\n");
    }

private:
    Thermocouple_t thermocouple;
} WellerController;

#endif //SOLDER_WELLER_H
