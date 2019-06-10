/*
 * LCD_cfg.h
 *
 *  Created on: Mar 23, 2019
 *      Author: zizo rashad
 */

#ifndef LCD_CFG_H_
#define LCD_CFG_H_

// connect data into 8 or 4 bit pins
#define LCD_interface 4

#define LCD_PORT 0
#define CONTROL_PORT 1
#define RS	0
#define RW	1
#define E	2

//#if (LCD_interface == 4)
// in 4-bit mode
	#define D4	4
	#define D5	5
	#define D6	6
	#define D7	7

#endif /* LCD_CFG_H_ */
