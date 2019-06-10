/*
 * RTOS.main.c
 *
 *  Created on: ??þ/??þ/????
 *      Author: Zizo Rashad
 */
#include"Types.h"
#include"DIO.h"
#include"LCD.h"
#include"TIMER0.h"
#include"TIMER1.h"
#include"TIMER2.h"
#include"registers.h"
//#include"RTOS.h"

typedef struct {
	u8 periodicity;
	u8 periority;
	void (*PF)(void);
}TCB;

#define num_task 3
TCB arr[num_task];
volatile u8 counter = 0;




void led_blink200 (void)
{
	DIO_vidSetPinDirection(C,7,1);
	DIO_vidTogllePin(C,7);
}

void led_blink400 (void)
{
	DIO_vidSetPinDirection(C,6,1);
	DIO_vidTogllePin(C,6);
}

void led_blink600 (void)
{
	DIO_vidSetPinDirection(C,5,1);
	DIO_vidTogllePin(C,5);
}

void rotating_lcd(void)
{
	u8 row1[16]={'A','b','d','e','l','a','z','i','z',7,7,7,7,7,7,7};
	// printing the English name in row 1
	LCD_vidSetPosition(1,1);
	for (u8 i=0;i<16;i++)
	{
		LCD_vidWriteChar(row1[i]);
	}
}

void RTOS_vidTaskCreate(u8 periority,u8 periodicity,void (*PF)(void))
{
//	static u8 index = 0;
	arr[periority-1].periodicity=periodicity;
	arr[periority-1].periority=periority;
	arr[periority-1].PF=PF;
//	index++;
}

void RTOS_vidTaskdelete(u8 periority)
// periority starts from 1 and increase
{
	for (u8 i=periority-1;i<=num_task;i++)
	{
		if (i==num_task)
		{
			arr[i].periority=0;
			arr[i].PF=0;
			arr[i].periodicity=0;
		}else
		{
			arr[i]=arr[i+1];
			arr[i].periority--;
		}
	}
}
void scheduler(void)
{
	for (u8 i = 0;i<num_task;i++)
	{
		if (arr[i].PF==0)
		{
			continue;
		}
		else if (counter % arr[i].periodicity==0)
		{
			arr[i].PF();
		}else {}
	}
}

void RTOS_vidTickTime(void)
{
	counter ++;
	scheduler();
}

/*void interrupt_timer0(void)
{
	counter ++;
	scheduler();
}


void  __vector_10(void)__attribute__( (signal,__INTR_ATTRS) );
void __vector_10(void)
{
	counter ++;
	scheduler();
}
*/


int main(void)
{
	LCD_vidInit();
	RTOS_vidTaskCreate(1,6,&led_blink200);
	RTOS_vidTaskCreate(2,12,&led_blink400);
	RTOS_vidTaskCreate(3,18,&led_blink600);
//	RTOS_vidInit();
//	RTOS_vidTaskdelete(1);

//	TIMER0_vidSetCallBack(RTOS_vidTickTime);
	TIMER1_vidSetCallBack(RTOS_vidTickTime);
//	TIMER2_vidSetCallBack(RTOS_vidTickTime);

//	TIMER0_vidCtcTimeOn(5,0,250);
//	TIMER0_vidNormal(5,0);
//	TIMER1_Init(0,0,0,7,0);
//	TIMER1_u32Normal(2);

	TIMER1_u32Ctc(5,250,WAVE1,TIMER1B);

//	TIMER2_vidNormal(7,0);
//	TIMER2_vidCtcTimeOn(7,0,250);
	while(1)
	{
		LCD_vidSetPosition(1,1);
		LCD_vidWriteNumber(counter);
		LCD_vidWriteString((u8*) "   good  ");
//		LCD_vidSetPosition(2,1);
//		LCD_vidWriteNumber(TCCR1A);
//		LCD_vidSetPosition(2,5);
//		LCD_vidWriteNumber(TCCR1B);

	}
	return 0;
}
