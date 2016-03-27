#include "LCD_trans.h"
#include <string.h>
#include <stdio.h>
#include "defines.h"
#include "hd44780.h"

#include "stm32f1xx_hal.h"

volatile uint8_t CbHead = 0;
volatile uint8_t CbTail = 0;

volatile LCD_trans LCDBuffer[LCD_MAXTRANS];

volatile uint8_t lcd_timeproc_enabled = 0;

void LCD_4us_timeproc(void) {

    static uint8_t pos = 0;           //Pozycja w polu danych aktualnej transakcji
    static uint8_t nibble = 0;           //Kt�ra tetrada jest wysy�ana
    if (!lcd_timeproc_enabled)return;

    //GPIO_SetBits(LCD_GPIO,LCD_EN);

    if (CbHead == CbTail) {//bufor pusty
        lcd_timeproc_enabled = 0;
    } else {                                 //Jest transakcja do zrealizowania
        uint8_t dat = LCDBuffer[CbTail].data[pos];
        if (nibble == 0) dat >>= 4;
        hd44780_outnibble_nowait(dat & 0x0F, pos != 0);    //Zapisujemy rejestr steruj�cy lub dane
        nibble = (nibble + 1) % 2;
        if (nibble == 0) ++pos; //Co drug� tetrad� zwi�kszamy pozycj� bufora
        if (pos >= LCDBuffer[CbTail].len) {
            CbTail = (CbTail + 1) % LCD_MAXTRANS;
            pos = 0;
            nibble = 0;
        }
        //CLR(OUT, HD44780_E);
        volatile int i = 100;
        while (i--) { }
        HAL_GPIO_WritePin(LCD_GPIO, LCD_EN, GPIO_PIN_RESET);
        //char bufor[20];
        //sprintf(bufor,"-%u %u %u %u\r\n",CbTail, CbHead, LCDBuffer[CbTail].len, pos);
        //uart_puts(bufor);
    }
}

void LCD_init() {

    hd44780_init();                            //Podstawowa inicjalizacja modu�u
    hd44780_outcmd(HD44780_CLR);                //Wyczy�� pami�� DDRAM
    HAL_Delay(2);
    hd44780_outcmd(HD44780_ENTMODE(1, 0));    //Tryb autoinkrementacji AC
    HAL_Delay(2);
    hd44780_outcmd(HD44780_DISPCTL(1, 0, 0));    //W��cz wy�wietlacz, wy��cz kursor
    HAL_Delay(2);
}


bool LCD_PutText(uint8_t x, uint8_t y, char *txt) {
    __disable_irq();
    uint8_t CbHeadTmp = (CbHead + 1) % LCD_MAXTRANS;
    if (CbHeadTmp == CbTail) {
        __enable_irq();
        return false;
    }
    switch (y) {
        case 0:
        default:
            y = 0x00;
            break;
        case 1:
            y = 0x40;
            break;
        case 2:
            y = 0x14;
            break;
        case 3:
            y = 0x54;
            break;
    }
    LCDBuffer[CbHead].data[0] = HD44780_DDADDR(x + y);   //Ustaw adres w DDRAM
    strcpy((char *) (&LCDBuffer[CbHead].data[1]), txt);  //Skopiuj dane tekstowe + NULL
    LCDBuffer[CbHead].len = strlen(txt) + 1;    //D�ugo�� tekstu + pozycji + NULL - 1
    lcd_timeproc_enabled = 1;     //Preskaler 1 - odblokuj timer
    CbHead = CbHeadTmp;
    __enable_irq();
    //char bufor[20];
    //sprintf(bufor,"+%u %u\r\n",CbTail, CbHead);
    //uart_puts(bufor);
    return true;
}
