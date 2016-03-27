#ifndef LCD_trans_h__
#define LCD_trans_h__

#include <stdbool.h>
#include <stdint.h>

#define LCD_MAXTRANS 8           //Maksymalna liczba pami�tanych transakcji
#define LCD_MAX_DATA 17

extern volatile uint8_t CbHead;
extern volatile uint8_t CbTail;

extern volatile uint8_t lcd_timeproc_enabled;

typedef struct
{
	uint8_t		len;					//D�ugo�� polecenia
	uint8_t		data[LCD_MAX_DATA+2];		//Dane transakcji
} LCD_trans;



void LCD_init();                                   //Inicjalizacja LCD i transakcji
bool LCD_PutText(uint8_t x, uint8_t y, char *txt); //Wy�wietl napis na LCD

void LCD_4us_timeproc(void);

void LCD_def_chr(uint8_t location, const uint8_t *ptr);

#endif // LCD-trans_h__
