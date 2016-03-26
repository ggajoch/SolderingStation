#include <stdint.h>
#include "HAL.h"

int main(void) {
    WellerHAL hal;
    float val = 0;
    while (1) {
        HAL_UART_Transmit(&hal.huart1, (uint8_t *) "test\r\n", 6, 1000);
        for (val = 0; val < 100; val += 1) {
            hal.setBacklight(val);
            HAL_Delay(100);
        }
        for (val = 0; val < 100; val += 1) {
            hal.setContrast(val);
            HAL_Delay(100);
        }
        HAL_Delay(100);
    }
}
