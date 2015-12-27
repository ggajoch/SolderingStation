/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <joerg@FreeBSD.ORG> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.        Joerg Wunsch
 * Modified for XMEGA support by Tomasz Francuz
 * ----------------------------------------------------------------------------
 *
 * HD44780 LCD display driver
 *
 * The LCD controller is used in 4-bit mode with a full bi-directional
 * interface (i.e. R/~W is connected) so the busy flag can be read.
 *
 * $Id: hd44780.c,v 1.3.2.1 2009/06/25 20:21:16 joerg_wunsch Exp $
 */
#include "stm32f10x.h"
#include "system_stm32f10x.h"
#include "stm32f10x_conf.h"
#include "defines.h"
#include "Delay/delay_ms.h"

#include <stdbool.h>
#include <stdint.h>



#include "hd44780.h"
//#include "PinMacros.h"


#define HD44780_BUSYFLAG 0x80

void write_4b(uint8_t data){
	GPIO_WriteBit(LCD_GPIO,LCD_D4,(BitAction)((data&(1<<0))!=0));
	GPIO_WriteBit(LCD_GPIO,LCD_D5,(BitAction)((data&(1<<1))!=0));
	GPIO_WriteBit(LCD_GPIO,LCD_D6,(BitAction)((data&(1<<2))!=0));
	GPIO_WriteBit(LCD_GPIO,LCD_D7,(BitAction)((data&(1<<3))!=0));
}

/*
 * Send one pulse to the E signal (enable).  Mind the timing
 * constraints.  If readback is set to true, read the HD44780 data
 * pins right before the falling edge of E, and return that value.
 */
static inline uint8_t
hd44780_pulse_e(bool readback) __attribute__((always_inline));

static inline uint8_t
hd44780_pulse_e(bool readback)
{
  uint8_t x;

  GPIO_SetBits(LCD_GPIO,LCD_EN);
  /*
   * Guarantee at least 500 ns of pulse width.  For high CPU
   * frequencies, a delay loop is used.  For lower frequencies, NOPs
   * are used, and at or below 1 MHz, the native pulse width will
   * already be 1 us or more so no additional delays are needed.
   */
  _delay_ms(1);
  /*
   * When reading back, we need one additional NOP, as the value read
   * back from the input pin is sampled close to the beginning of a
   * CPU clock cycle, while the previous edge on the output pin is
   * generated towards the end of a CPU clock cycle.
   */
  if (readback)
    x = GPIO_ReadInputDataBit(LCD_GPIO,LCD_D4);
  else
    x = 0;
  GPIO_ResetBits(LCD_GPIO,LCD_EN);


  return x;
}

/*
 * Send one nibble out to the LCD controller.
 */
//static void
void
hd44780_outnibble(uint8_t n, uint8_t rs)
{
  //CLR(OUT, HD44780_RW);
  GPIO_WriteBit(LCD_GPIO,LCD_RS,(BitAction)rs);
  write_4b(n);
  (void)hd44780_pulse_e(false);
}

void hd44780_outnibble_nowait(uint8_t n, uint8_t rs)
{
  //CLR(OUT, HD44780_E);              //Wyzeruj sygna� Enable i zatrza�nij dane
  //CLR(OUT, HD44780_RW);
	GPIO_WriteBit(LCD_GPIO,LCD_RS,(BitAction)rs);
  	write_4b(n);
  	GPIO_SetBits(LCD_GPIO,LCD_EN);//Ustaw enable
}

/*
 * Send one byte to the LCD controller.  As we are in 4-bit mode, we
 * have to send two nibbles.
 */
void
hd44780_outbyte(uint8_t b, uint8_t rs)
{
  hd44780_outnibble(b >> 4, rs);
  hd44780_outnibble(b & 0xf, rs);
}

/*
 * Read one nibble from the LCD controller.
 */
static uint8_t
hd44780_innibble(uint8_t rs)
{
  /*uint8_t x;

  SET(OUT, HD44780_RW);
  ASSIGN(DIR, HD44780_D4, 0x00);
  if (rs)
    SET(OUT, HD44780_RS);
  else
    CLR(OUT, HD44780_RS);
  x = hd44780_pulse_e(true);
  ASSIGN(DIR, HD44780_D4, 0x0F);
  CLR(OUT, HD44780_RW);*/

  return 0;//not using RW pin
}

/*
 * Read one byte (i.e. two nibbles) from the LCD controller.
 */
uint8_t
hd44780_inbyte(uint8_t rs)
{
  uint8_t x;

  x = hd44780_innibble(rs) << 4;
  x |= hd44780_innibble(rs);

  return x;
}

/*
 * Wait until the busy flag is cleared.
 */
void hd44780_wait_ready(bool longwait){
  if (longwait)
    _delay_ms(2);
  else
    _delay_ms(1);
}

/*
 * Initialize the LCD controller.
 *
 * The initialization sequence has a mandatory timing so the
 * controller can safely recognize the type of interface desired.
 * This is the only area where timed waits are really needed as
 * the busy flag cannot be probed initially.
 */
void hd44780_init(void){

	GPIO_InitTypeDef GPIO_InitStructure;
	// inicjowanie pinów portów ustalonych do podłączenia z wyświetlaczem LCD
	// ustawienie wszystkich jako wyjścia
	/* GPIOB dla LCD - zegar */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	/* GPIOB dla LCD - konfiguracja */
	GPIO_InitStructure.GPIO_Pin = LCD_ALL;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(LCD_GPIO, &GPIO_InitStructure);

	GPIO_ResetBits(LCD_GPIO, LCD_ALL);



	_delay_ms(40);		/* 40 ms needed for Vcc = 2.7 V */
	hd44780_outnibble(HD44780_FNSET(1, 0, 0) >> 4, 0);
	_delay_ms(10);
	hd44780_outnibble(HD44780_FNSET(1, 0, 0) >> 4, 0);
	_delay_ms(10);
	hd44780_outnibble(HD44780_FNSET(1, 0, 0) >> 4, 0);
	_delay_ms(2);

	hd44780_outnibble(HD44780_FNSET(0, 1, 0) >> 4, 0);
	_delay_ms(2);
	hd44780_outcmd(HD44780_FNSET(0, 1, 0));
	_delay_ms(2);
	hd44780_outcmd(HD44780_DISPCTL(0, 0, 0));
	_delay_ms(2);
}

/*
 * Prepare the LCD controller pins for powerdown.
 */
void hd44780_powerdown(void){
	GPIO_ResetBits(LCD_GPIO,LCD_ALL);
}

void hd44780_def_chr(uint8_t location, const uint8_t *ptr){
	if(location<8){
		hd44780_outcmd(HD44780_CGADDR(location*8));
		hd44780_wait_ready(1);
		uint8_t i;
		for(i=0;i<8;i++){
			hd44780_outdata(ptr[i]);
			hd44780_wait_ready(1);
		}

	}
}
