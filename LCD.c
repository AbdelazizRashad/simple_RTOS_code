/*
 * LCD.c
 *
 *  Created on: Mar 23, 2019
 *      Author: zizo rashad
 */
#include"types.h"
#include"LCD_cfg.h"
#include"LCD.h"
#include"DIO.h"
#include"macros.h"
#define F_CPU 8000000
#include <util/delay.h>

void LCD_vidWriteChar(u8 ch)
// writing character to LCD
{
	// setting the lcd ready to write data on it
	DIO_vidSetPinValue(CONTROL_PORT, RW ,0);
	DIO_vidSetPinValue(CONTROL_PORT , RS ,1);

	switch (LCD_interface)
	{
		case 8:		// 8-bit MODE
			DIO_vidSetPortValue(LCD_PORT,ch);
			DIO_vidSetPinValue(CONTROL_PORT , E ,1);
			_delay_ms(1);
			DIO_vidSetPinValue(CONTROL_PORT, E , 0 );
			_delay_ms(1);
			break;
		case 4:		// 4-bit MODE
			// firstly send the low 4 digit of ch
			DIO_vidSetPinValue(LCD_PORT, D4, GET_BIT(ch,4) );
			DIO_vidSetPinValue(LCD_PORT, D5, GET_BIT(ch,5) );
			DIO_vidSetPinValue(LCD_PORT, D6, GET_BIT(ch,6) );
			DIO_vidSetPinValue(LCD_PORT, D7, GET_BIT(ch,7) );
			// enable triger for recieve data
			DIO_vidSetPinValue(CONTROL_PORT , E ,1);
			_delay_ms(1);
			DIO_vidSetPinValue(CONTROL_PORT, E , 0 );
			_delay_ms(1);

			//secondly send the high 4 digit of ch
			DIO_vidSetPinValue(LCD_PORT, D4, GET_BIT(ch,0) );
			DIO_vidSetPinValue(LCD_PORT, D5, GET_BIT(ch,1) );
			DIO_vidSetPinValue(LCD_PORT, D6, GET_BIT(ch,2) );
			DIO_vidSetPinValue(LCD_PORT, D7, GET_BIT(ch,3) );
			//  enable triger for recieve data
			DIO_vidSetPinValue(CONTROL_PORT , E ,1);
			_delay_ms(1);
			DIO_vidSetPinValue(CONTROL_PORT, E , 0 );
			_delay_ms(1);
			break;
	}
}

void LCD_vidWriteCommand(u8 command)
{
	DIO_vidSetPinValue(CONTROL_PORT , RW ,0);
	DIO_vidSetPinValue(CONTROL_PORT , RS ,0);

	switch (LCD_interface)
	{
		case 8:		// 8-bit MODE
			DIO_vidSetPortValue(LCD_PORT,command);
			DIO_vidSetPinValue(CONTROL_PORT , E ,1);
			_delay_ms(1);
			DIO_vidSetPinValue(CONTROL_PORT, E , 0 );
			_delay_ms(1);
			break;
		case 4:		// 4-bit MODE
			// firstly send the low 4 digit of command
			DIO_vidSetPinValue(LCD_PORT, D4, GET_BIT(command,4) );
			DIO_vidSetPinValue(LCD_PORT, D5, GET_BIT(command,5) );
			DIO_vidSetPinValue(LCD_PORT, D6, GET_BIT(command,6) );
			DIO_vidSetPinValue(LCD_PORT, D7, GET_BIT(command,7) );
			//enable triger for recieve data
			DIO_vidSetPinValue(CONTROL_PORT , E ,1);
			_delay_ms(1);
			DIO_vidSetPinValue(CONTROL_PORT, E , 0 );
			_delay_ms(1);

			//secondly send the high 4 digit of command
			DIO_vidSetPinValue(LCD_PORT, D4, GET_BIT(command,0) );
			DIO_vidSetPinValue(LCD_PORT, D5, GET_BIT(command,1) );
			DIO_vidSetPinValue(LCD_PORT, D6, GET_BIT(command,2) );
			DIO_vidSetPinValue(LCD_PORT, D7, GET_BIT(command,3) );
			// enable triger for recieve data
			DIO_vidSetPinValue(CONTROL_PORT , E ,1);
			_delay_ms(1);
			DIO_vidSetPinValue(CONTROL_PORT, E , 0 );
			_delay_ms(1);
			break;
	}
}

void LCD_vidInit(void)
// Initializing the LCD >>> First step of using LCD
{
	DIO_vidSetPinDirection(CONTROL_PORT,RW,1);
	DIO_vidSetPinDirection(CONTROL_PORT,RS,1);
	DIO_vidSetPinDirection(CONTROL_PORT,E,1);
	_delay_ms(50);

	switch (LCD_interface)
	{
		case 8:		// 8-bit MODE
			DIO_vidSetPortDirection(LCD_PORT, 255);
			LCD_vidWriteCommand(0b00111000);
			_delay_ms(1);
			break;
		case 4:		// 8-bit MODE
			// setting the 4 bit as o/p for writing data and command to LCD
			DIO_vidSetPinDirection(LCD_PORT,D4,1);
			DIO_vidSetPinDirection(LCD_PORT,D5,1);
			DIO_vidSetPinDirection(LCD_PORT,D6,1);
			DIO_vidSetPinDirection(LCD_PORT,D7,1);
			// making the initialization due to datesheet

			// step 1 send just 4 bit once
			DIO_vidSetPinValue(LCD_PORT, D4, 0);
			DIO_vidSetPinValue(LCD_PORT, D5, 1);
			DIO_vidSetPinValue(LCD_PORT, D6, 0);
			DIO_vidSetPinValue(LCD_PORT, D7, 0);
			// enable triger for recieve data
			DIO_vidSetPinValue(CONTROL_PORT , E ,1);
			_delay_ms(1);
			DIO_vidSetPinValue(CONTROL_PORT, E , 0 );
			_delay_ms(1);

			// step 2 send function set
			 LCD_vidWriteCommand(0b00101000);
			 _delay_ms(1);
			break;
	}

	/* common initialization commands in both modes (4-bit and 8-bit mode) */
	// Display on/off
	LCD_vidWriteCommand(0b00001100);
	_delay_ms(1);
	// Display Clear
	LCD_vidWriteCommand(0b00000001);

}


u8 LCD_u8ReadData(void)
// m4 naf3a 5als
// reading data
{
	u8 data = 0;
	DIO_vidSetPinValue(CONTROL_PORT , RW ,1);
	DIO_vidSetPinValue(CONTROL_PORT , RS ,1);
	DIO_vidSetPortDirection(LCD_PORT, 0);
	DIO_vidSetPinValue(CONTROL_PORT , E ,1);
	_delay_ms(1);
	DIO_vidSetPinValue(CONTROL_PORT, E , 0 );
	_delay_ms(1);
	data = DIO_u8ReadPortValue(LCD_PORT);
	DIO_vidSetPinValue(CONTROL_PORT , E ,1);
	_delay_ms(1);
	DIO_vidSetPinValue(CONTROL_PORT, E , 0 );
	_delay_ms(1);
/*
	DIO_vidSetPinValue(CONTROL_PORT , RW ,0);
	DIO_vidSetPinValue(CONTROL_PORT , RS ,0);
	DIO_vidSetPortDirection(LCD_PORT, 255);
*/
	return data ;
}

u8 LCD_u8ReadPosition(void)
// m4 naf3a 5als
{
	u8 position = 0;
	DIO_vidSetPinValue(CONTROL_PORT , RW ,1);
	DIO_vidSetPinValue(CONTROL_PORT , RS ,0);
	DIO_vidSetPortDirection(LCD_PORT, 0);
	position = DIO_u8ReadPortValue(LCD_PORT);
	if ( GET_BIT(position , 7) == 0 )
		SET_BIT(position,7);
	else {}
/*
	DIO_vidSetPinValue(CONTROL_PORT , RW ,0);
	DIO_vidSetPinValue(CONTROL_PORT , RS ,0);
	DIO_vidSetPortDirection(LCD_PORT, 255);
*/
	return position;
}

void LCD_vidWriteString(u8 * arr)
{
	u8 i=0;
	while (arr[i] != 0)
	{
		LCD_vidWriteChar(arr[i]);
		i++;
	}
}

void LCD_vidWriteNumber(u64 number)
{
	u64 number_copy=1;
	if (number == 0)
		LCD_vidWriteChar(48);
	else
	{
		// copying reversing number by adding 1 to left
		while (number != 0)
		{
			number_copy = number_copy *10 + number%10;
			number /= 10;
		}
		// write the number after reverse it except the last digit 1
		while (number_copy != 1 )
		{
			LCD_vidWriteChar( (u8) (number_copy % 10) + 48);
			number_copy /= 10;
		}
	}
}
void LCD_vidWriteFloatNumber(f64 number)
{
	u32 left;
	u32 right;
	f64 right_f;
	// in case number is negative ... printing the - value first
	if (number < 0)
	{
		number *= (f64) -1;
		LCD_vidWriteChar('-');
	}
	else {}
	// getting real number from the floating number and printing it
	left = (u32) number;
	LCD_vidWriteNumber(left);
	// getting fraction number in the floating number
	right_f =  number - (f64) left;
	right_f = 100000 * right_f;
	right = (u32) right_f ;
	// when the fraction not equal zero printing it after printing the dot char'.'
	if (right > 0)
	{
		while ((right % 10 ) == 0 && (right/10) !=0 )
		{
				right /= 10;
		}
		LCD_vidWriteChar('.');
		LCD_vidWriteNumber(right);

		}
}

void LCD_vidSetPosition(u8 row, u8 col)
// row is 1 or 2
// col from 1 to 16
{
	switch (row)
	{
	case 1:
		// 128 is the DDRAM address of first position in the row 1
		// the next 15 address's is increasing by step 1
		LCD_vidWriteCommand(128+(col-1));
		break;
	case 2:
		// 192 is the DDRAM address of first position in row 2
		// the next 15 address's is increasing by step 1
		LCD_vidWriteCommand(192+(col-1));
		break;
	default :
		break;

	}
}


void LCD_vidDisplayClr(void)
// clear all data in DDRAM and return home
{
	LCD_vidWriteCommand(0b00000001);
}

void LCD_vidDisplayHome(void)
//return home without change of data in DDRAM
{
	LCD_vidWriteCommand(0b00000010);
}

void LCD_vidSetEntryMode (u8 shift, u8 direction)
/* shift (S/C) >> 1 for shift the display , 0 for shift the cursor only
 * direction (R/L) >> 1 for shift right , 0 for shift left
 */
{
	u8 x = 4;
	if (shift == 1)
		SET_BIT(x,1);
	else if (shift == 0)
	{
		CLR_BIT(x,1);
	}
	else {}

	if (direction == 1)
		SET_BIT(x,0);
	else if (direction == 0)
	{
		CLR_BIT(x,0);
	}
	else {}

	LCD_vidWriteCommand(x);
}

void LCD_vidCursorAndBlinking(u8 cursor, u8 blink)
/* cursor >> 1 for on and 0 for off
 * blink >> 1 for on and 0 for off
 */
{
	u8 x = 12;
	if (cursor == 1)
		SET_BIT(x,1);
	else if (cursor == 0)
	{
		CLR_BIT(x,1);
	}
	else {}

	if (blink == 1)
		SET_BIT(x,0);
	else if (cursor == 0)
	{
		CLR_BIT(x,0);
	}
	else {}
	LCD_vidWriteCommand(x);
}
