/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <joerg@FreeBSD.ORG> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.        Joerg Wunsch
 * ----------------------------------------------------------------------------
 *
 * General stdiodemo defines
 *
 * $Id: defines.h,v 1.2.2.1 2009/06/25 20:21:15 joerg_wunsch Exp $
 */
#ifndef LCD_DEFINES_H
#define LCD_DEFINES_H

#define LCD_RS GPIO_Pin_0
#define LCD_EN GPIO_Pin_1
#define LCD_D4 GPIO_Pin_2
#define LCD_D5 GPIO_Pin_3
#define LCD_D6 GPIO_Pin_4
#define LCD_D7 GPIO_Pin_5



#define LCD_ALL LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7 | LCD_EN | LCD_RS

#define LCD_GPIO GPIOC

#endif
