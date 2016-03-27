#ifndef HD44780_H_
#define HD44780_H_

#include "stm32f1xx_hal.h"
#include "defines.h"

namespace HD44780 {

#define HD44780_CLEAR                   0x01

#define HD44780_HOME                    0x02

#define HD44780_ENTRY_MODE              0x04
    #define HD44780_EM_SHIFT_CURSOR        0
    #define HD44780_EM_SHIFT_DISPLAY       1
    #define HD44780_EM_DECREMENT           0
    #define HD44780_EM_INCREMENT           2

#define HD44780_DISPLAY_ONOFF           0x08
    #define HD44780_DISPLAY_OFF            0
    #define HD44780_DISPLAY_ON             4
    #define HD44780_CURSOR_OFF             0
    #define HD44780_CURSOR_ON              2
    #define HD44780_CURSOR_NOBLINK         0
    #define HD44780_CURSOR_BLINK           1

#define HD44780_DISPLAY_CURSOR_SHIFT    0x10
    #define HD44780_SHIFT_CURSOR           0
    #define HD44780_SHIFT_DISPLAY          8
    #define HD44780_SHIFT_LEFT             0
    #define HD44780_SHIFT_RIGHT            4

#define HD44780_FUNCTION_SET            0x20
    #define HD44780_FONT5x7                0
    #define HD44780_FONT5x10               4
    #define HD44780_ONE_LINE               0
    #define HD44780_TWO_LINE               8
    #define HD44780_4_BIT                  0
    #define HD44780_8_BIT                 16

#define HD44780_CGRAM_SET                0x40

#define HD44780_DDRAM_SET                0x80


    void _LCD_SetRS(bool state) {
        HAL_GPIO_WritePin(LCD_GPIO, LCD_RS, (GPIO_PinState) state);
    }

    void _LCD_SetEN(bool state) {
        HAL_GPIO_WritePin(LCD_GPIO, LCD_EN, (GPIO_PinState) state);
    }

    void _LCD_Write_NOBLOCK(uint8_t data) {
        HAL_GPIO_WritePin(LCD_GPIO, LCD_D0, (GPIO_PinState) ((data & (1 << 0)) != 0));
        HAL_GPIO_WritePin(LCD_GPIO, LCD_D1, (GPIO_PinState) ((data & (1 << 1)) != 0));
        HAL_GPIO_WritePin(LCD_GPIO, LCD_D2, (GPIO_PinState) ((data & (1 << 2)) != 0));
        HAL_GPIO_WritePin(LCD_GPIO, LCD_D3, (GPIO_PinState) ((data & (1 << 3)) != 0));
        HAL_GPIO_WritePin(LCD_GPIO, LCD_D4, (GPIO_PinState) ((data & (1 << 4)) != 0));
        HAL_GPIO_WritePin(LCD_GPIO, LCD_D5, (GPIO_PinState) ((data & (1 << 5)) != 0));
        HAL_GPIO_WritePin(LCD_GPIO, LCD_D6, (GPIO_PinState) ((data & (1 << 6)) != 0));
        HAL_GPIO_WritePin(LCD_GPIO, LCD_D7, (GPIO_PinState) ((data & (1 << 7)) != 0));
    }

    void _LCD_Write(uint8_t data) {
        HAL_Delay(1);
        _LCD_SetEN(true);
        HAL_Delay(1);
        _LCD_Write_NOBLOCK(data);
        HAL_Delay(1);
        _LCD_SetEN(false);
        HAL_Delay(1);
    }


    void LCD_WriteCommand(unsigned char commandToWrite) {
        _LCD_SetRS(false);
        HAL_Delay(1);
        _LCD_Write(commandToWrite);
    }

    void LCD_WriteData(unsigned char dataToWrite) {
        _LCD_SetRS(true);
        HAL_Delay(1);
        _LCD_Write(dataToWrite);
    }

    void LCD_WriteText(char *text) {
        while (*text)
            LCD_WriteData(*text++);
    }

    void LCD_GoTo(unsigned char x, unsigned char y) {
        LCD_WriteCommand(HD44780_DDRAM_SET | (x + (0x40 * y)));
    }

    void LCD_Clear(void) {
        LCD_WriteCommand(HD44780_CLEAR);
        HAL_Delay(2);
    }

    void LCD_Home(void) {
        LCD_WriteCommand(HD44780_HOME);
        HAL_Delay(2);
    }

    void LCD_Initalize(void) {
        HAL_Delay(50);

        _LCD_SetRS(false);
        _LCD_SetEN(false);

        for (uint8_t i = 0; i < 3; i++) {
            _LCD_Write(0x3F);
            HAL_Delay(5);
        }

        LCD_WriteCommand(HD44780_FUNCTION_SET | HD44780_FONT5x7 | HD44780_TWO_LINE |
                         HD44780_8_BIT);
        LCD_WriteCommand(HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_OFF);
        LCD_WriteCommand(HD44780_CLEAR);
        HAL_Delay(2);
        LCD_WriteCommand(HD44780_ENTRY_MODE | HD44780_EM_SHIFT_CURSOR |
                         HD44780_EM_INCREMENT);
        LCD_WriteCommand(HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_ON | HD44780_CURSOR_OFF |
                         HD44780_CURSOR_NOBLINK);
    }
}

#endif // HD44780_H_
