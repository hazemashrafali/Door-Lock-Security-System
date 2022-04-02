/*
 * TIMER.c
 *
 *  Created on: Mar 20, 2022
 *      Author: HAZEM-PC
 */


#include "TIMER.h"
#include <avr/io.h>
#include <avr/interrupt.h>


/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
/* Global variables to hold the address of the call back function in the application */
static volatile void (*g0_callBackPtr)(void) = NULL_PTR;
static volatile void (*g1_callBackPtr)(void) = NULL_PTR;
static volatile void (*g2_callBackPtr)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(TIMER0_OVF_vect)
{
	if(g0_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application */
		(*g0_callBackPtr)();
	}
}
ISR(TIMER0_COMP_vect)
{
	if(g0_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application */
		(*g0_callBackPtr)();
	}
}
ISR(TIMER1_OVF_vect)
{
	if(g1_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application */
		(*g1_callBackPtr)();
	}
}
ISR(TIMER1_COMPA_vect)
{
	if(g1_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application */
		(*g1_callBackPtr)();
	}
}
ISR(TIMER2_OVF_vect)
{
	if(g2_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application */
		(*g2_callBackPtr)();
	}
}
ISR(TIMER2_COMP_vect)
{
	if(g2_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application */
		(*g2_callBackPtr)();
	}
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description: Function to initialize timer0 to normal mode.
 */
static void Timer0_Init_normal_mode(Timer_configtytpe **timer_type)
{
	TCNT0 = (*timer_type)->initial_value.uint8_value; //Set Timer initial value to 0
	TIMSK |= (1<<TOIE0); // Enable Timer0 Overflow Interrupt
	/* Configure the timer control register
	 * 1. Non PWM mode FOC0=1
	 * 2. Normal Mode WGM01=0 & WGM00=0
	 * 3. Normal Mode COM00=0 & COM01=0
	 * 4. clock = pre_scaler value CS00,CS01,CS02
	 */
	TCCR0 = (1<<FOC0);
	TCCR0 = (TCCR0 & 0xF8) | ((*timer_type)->CLK_prescaler.prescaler_0_1);
}

/*
 * Description: Function to initialize timer0 to CTC mode.
 */
static void Timer0_Init_CTC_Mode(Timer_configtytpe **timer_type)
{
	TCNT0 = (*timer_type)->initial_value.uint8_value;    // Set Timer initial value to 0
	OCR0  = (*timer_type)->compare_value.uint8_value;    // Set Compare Value
	TIMSK |= (1<<OCIE0); // Enable Timer0 Compare Interrupt
	/* Configure timer0 control register
	 * 1. Non PWM mode FOC0=1
	 * 2. CTC Mode WGM01=1 & WGM00=0
	 * 3. set OC0 to toggle on compare match so COM00=1 & COM01=0
	 * 4. clock = pre_scaler value
	 */
	TCCR0 = (1<<FOC0) | (1<<WGM01) | (1<<COM00);
	TCCR0 = (TCCR0 & 0xF8) | ((*timer_type)->CLK_prescaler.prescaler_0_1);
}

/*
 * Description: Function to initialize timer1 to normal mode.
 */
static void Timer1_Init_normal_mode(Timer_configtytpe **timer_type)
{
	TCNT1 = (*timer_type)->initial_value.uint16_value; //Set Timer initial value to 0
	TIMSK |= (1<<TOIE1); // Enable Timer0 Overflow Interrupt
	/* Configure the timer control register
	 * 1. Non PWM mode FOC0=1
	 * 2. Normal Mode WGM10=0 & WGM11=0 & WGM12=0 & WGM13=0
	 * 3. Normal Mode COM1A1/B1=0 & COM1A0/B0=0
	 * 4. clock = pre_scaler value CS12,CS11,CS10
	 */
	TCCR1A = (1<<FOC1A) | (1<<FOC1B);
	TCCR1B = (TCCR1B & 0xF8) | ((*timer_type)->CLK_prescaler.prescaler_0_1);
}

/*
 * Description: Function to initialize timer1 to CTC mode.
 */
static void Timer1_Init_CTC_Mode(Timer_configtytpe **timer_type)
{
	TCNT1 = (*timer_type)->initial_value.uint16_value; //Set Timer initial value to 0
	OCR1A = (*timer_type)->compare_value.uint16_value; // set compare value OCR1A
	TIMSK |= (1<<OCF1A); // Enable Timer1 OUTPUT COMPARE A MATCH Interrupt
	/* Configure the timer control register
	 * 1. Non PWM mode FOC0=1
	 * 2. CTC Mode 4. WGM10=0 & WGM11=0 & WGM12=1 & WGM13=0
	 * 3. CTC toggle Mode for compare unit A. COM1A1=0 & COM1A0=1
	 * 4. clock = pre_scaler value CS12,CS11,CS10
	 */
	TCCR1A = (1<<FOC1A) | (1<<FOC1B) | (1<<COM1A0);
	TCCR1B = (1<<WGM12);
	TCCR1B = (TCCR1B & 0xF8) | ((*timer_type)->CLK_prescaler.prescaler_0_1);
}

/*
 * Description: Function to initialize timer2 to normal mode.
 */
static void Timer2_Init_normal_mode(Timer_configtytpe **timer_type)
{
	TCNT2 = (*timer_type)->initial_value.uint8_value; //Set Timer initial value to 0
	TIMSK |= (1<<TOIE2); // Enable Timer2 Overflow Interrupt
	/* Configure the timer2 control register
	 * 1. Non PWM mode FOC2=1
	 * 2. Normal Mode WGM21=0 & WGM20=0
	 * 3. Normal Mode COM20=0 & COM21=0
	 * 4. clock = pre_scaler value CS20,CS21,CS22
	 */
	TCCR2 = (1<<FOC2);
	TCCR2 = (TCCR2 & 0xF8) | ((*timer_type)->CLK_prescaler.prescaler_2);
}

/*
 * Description: Function to initialize timer2 to CTC mode.
 */
static void Timer2_Init_CTC_Mode(Timer_configtytpe **timer_type)
{
	TCNT2 = (*timer_type)->initial_value.uint8_value;    // Set Timer initial value to 0
	OCR2  = (*timer_type)->compare_value.uint8_value;    // Set Compare Value
	TIMSK |= (1<<OCIE2); // Enable Timer2 Compare Interrupt
	/* Configure timer2 control register
	 * 1. Non PWM mode FOC2=1
	 * 2. CTC Mode WGM21=1 & WGM20=0
	 * 3. set OC2 to toggle on compare match so COM20=1 & COM21=0
	 * 4. clock = pre_scaler value CS20,CS21,CS22
	 */
	TCCR2 = (1<<FOC2) | (1<<WGM21) | (1<<COM20);
	TCCR2 = (TCCR2 & 0xF8) | ((*timer_type)->CLK_prescaler.prescaler_2);
}

/*
 * Description: Function to set the Call Back function address for timer0 normal mode.
 */
void Timer0_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g0_callBackPtr = a_ptr;
}

/*
 * Description: Function to set the Call Back function address for timer1 normal mode.
 */
void Timer1_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g1_callBackPtr = a_ptr;
}

/*
 * Description: Function to set the Call Back function address for timer2 normal mode.
 */
void Timer2_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g2_callBackPtr = a_ptr;
}


/*
 * Description: Function to disable the timer.
 */
void Timer_Deinit(Timer_configtytpe *timer_type)
{
	if(timer_type->timer_id == TIMER_0)
	{
		TCCR0=0;  //clear timer0 CLK
		CLEAR_BIT(TIMSK,TOIE0);  //disable timer overflow interrupt
		CLEAR_BIT(TIMSK,OCIE0);  //disable timer compare interrupt
	}
	else if(timer_type->timer_id == TIMER_1)
	{
		TCCR1B=0; //clear timer1 CLK
		CLEAR_BIT(TIMSK,TOIE1);   //disable timer overflow interrupt
		CLEAR_BIT(TIMSK,OCIE1A);  //disable timer compare interrupt
	}
	else if(timer_type->timer_id == TIMER_2)
	{
		TCCR2=0; //clear timer2 CLK
		CLEAR_BIT(TIMSK,TOIE2);   //disable timer overflow interrupt
		CLEAR_BIT(TIMSK,OCIE2);   //disable timer compare interrupt
	}
}

/*
 * Description: Function to Enable the timer.
 */
void Timer_init(Timer_configtytpe *timer_type)
{
	switch(timer_type->timer_id)
	{
	case TIMER_0:
	{
		if(timer_type->timer_mode == Normal)
			Timer0_Init_normal_mode(&timer_type);
		else if(timer_type->timer_mode == CTC)
			Timer0_Init_CTC_Mode(&timer_type);
		else
		{
		}
	}
	break;
	case TIMER_1:
	{
		if(timer_type->timer_mode == Normal)
			Timer1_Init_normal_mode(&timer_type);
		else if(timer_type->timer_mode == CTC)
			Timer1_Init_CTC_Mode(&timer_type);
		else
		{
		}
	}
	break;
	case TIMER_2:
	{
		if(timer_type->timer_mode == Normal)
			Timer2_Init_normal_mode(&timer_type);
		else if(timer_type->timer_mode == CTC)
			Timer2_Init_CTC_Mode(&timer_type);
		else
		{
		}
	}
	break;
	default:
	{
	}
	}
}
void Timer0_delay_us(Timer_configtytpe *timer,uint16 delay_value)
{

	if(timer->timer_mode == Normal)
	{

	}
	else if(timer->timer_mode == CTC)
	{

	}
}
