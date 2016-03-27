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

#include "mxconstants.h"

#define LCD_RS GPIO_LCD_RS_Pin
#define LCD_EN GPIO_LCD_EN_Pin

#define LCD_D0 GPIO_LCD_D0_Pin
#define LCD_D1 GPIO_LCD_D1_Pin
#define LCD_D2 GPIO_LCD_D2_Pin
#define LCD_D3 GPIO_LCD_D3_Pin
#define LCD_D4 GPIO_LCD_D4_Pin
#define LCD_D5 GPIO_LCD_D5_Pin
#define LCD_D6 GPIO_LCD_D6_Pin
#define LCD_D7 GPIO_LCD_D7_Pin

#define LCD_ALL LCD_D0 | LCD_D1 | LCD_D2 | LCD_D3 | \
        LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7 | \
        LCD_EN | LCD_RS

#define LCD_GPIO GPIO_LCD_D7_GPIO_Port

#endif
