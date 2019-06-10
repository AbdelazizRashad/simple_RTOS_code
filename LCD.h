/*
 * LCD.h
 *
 *  Created on: Mar 23, 2019
 *      Author: zizo rashad
 */

#ifndef LCD_H_
#define LCD_H_

#include "types.h"

void LCD_vidWriteChar(u8 ch);
void LCD_vidWriteCommand(u8 ch);
void LCD_vidInit(void);
u8 LCD_u8ReadData(void);
u8 LCD_u8ReadPosition(void);
void LCD_vidWriteString(u8 * arr);
void LCD_vidWriteNumber(u64 number);
void LCD_vidWriteFloatNumber(f64 number);
void LCD_vidSetPosition(u8 row, u8 col);
void LCD_vidDisplayClr(void);
void LCD_vidDisplayHome(void);
void LCD_vidSetEntryMode (u8 shift, u8 direction);
void LCD_vidCursorAndBlinking(u8 cursor, u8 blink);

#endif /* LCD_H_ */
