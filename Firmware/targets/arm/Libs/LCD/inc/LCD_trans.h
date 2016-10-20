#ifndef LCD_TRANS_H_
#define LCD_TRANS_H_

#include "LCD_trans.h"
#include <string.h>
#include <LCDTransaction.h>
#include <cmsis_gcc.h>
#include "HD44780.h"
#include "cyclicBuffer.h"

namespace HD44780 {
    volatile uint8_t tick_enabled;
    LCDTransaction actual_data, *actual;
    CyclicBuffer_data<LCDTransaction, 50> buffer;

    void TimeTick(void) {
        if (!tick_enabled)
            return;

        if (actual != nullptr) {
            _LCD_SetRS(!actual->isCommand());
            _LCD_SetEN(true);
            _LCD_Write_NOBLOCK(actual->get());
            if (!actual->advance()) {
                actual = nullptr;
            }
            volatile uint8_t i = 100;
            while (i--) { }
            _LCD_SetEN(false);
        } else {
            if (buffer.isEmpty()) {
                tick_enabled = 0;
            } else {
                actual_data = buffer.get_unsafe();
                actual = &actual_data;
            }
        }
    }

    void Init() {
        LCD_Initalize();
    }


    void PutText(uint8_t x, uint8_t y, char *txt) {
        __disable_irq();

        ArrayHolder<uint8_t, 1> cmd;
        cmd.append(HD44780_DDRAM_SET | (x + (0x40 * y)));
        LCDTransaction position(true, cmd);
        buffer.append(position);

        ArrayHolder<uint8_t, 16> view((uint8_t *) txt, strlen(txt));
        LCDTransaction data(false, view);
        buffer.append(data);

        tick_enabled = 1;

        __enable_irq();
    }

    void Clear() {
        __disable_irq();

        ArrayHolder<uint8_t, 1> cmd;
        cmd.append(HD44780_CLEAR);
        LCDTransaction command(true, cmd);
        buffer.append(command);

        tick_enabled = 1;

        __enable_irq();
    }
}

#endif // LCD_TRANS_H_
