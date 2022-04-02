/*
 * TIMER.h
 *
 *  Created on: Mar 20, 2022
 *      Author: HAZEM-PC
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"
#include "gpio.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum{
	TIMER_0,TIMER_1,TIMER_2
}Timer_ID;

typedef enum{
	Normal,CTC
}Timer_mode;

typedef enum{
	No_CLK,CLK,CLK_8,CLK_64,CLK_256,CLK_1024
}Timer0_1_prescaler;

typedef enum{
	No_CLK_,CLK_,CLK_8_,CLK32_,CLK_64_,CLK128_,CLK_256_,CLK_1024_
}Timer2_prescaler;

typedef union{
	Timer0_1_prescaler prescaler_0_1;
	Timer2_prescaler prescaler_2;
}Timer_prescaler;

typedef union{
	uint8 uint8_value;
	uint16 uint16_value;
}Timer_value;

typedef struct{
	Timer_ID timer_id;
	Timer_mode timer_mode;
	Timer_value initial_value;
	Timer_value compare_value;
	Timer_prescaler CLK_prescaler;
}Timer_configtytpe;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

void Timer_init(Timer_configtytpe *timer_type); //Description: Function to Enable the timer.
void Timer0_setCallBack(void(*a_ptr)(void)); //Description: Function to set the Call Back function address for timer0
void Timer1_setCallBack(void(*a_ptr)(void)); //Description: Function to set the Call Back function address for timer1
void Timer2_setCallBack(void(*a_ptr)(void)); //Description: Function to set the Call Back function address for timer2
void Timer_Deinit(Timer_configtytpe *timer_type); //Description: Function to disable the timer.

#endif /* TIMER_H_ */
