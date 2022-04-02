/*
 * DCmotor.c
 *
 *  Created on: Feb 23, 2022
 *      Author: HAZEM-PC
 */


#include "gpio.h"
#include "avr/io.h"
#include "DCmotor.h"

void PWM_Timer0_Init(void)
{
	TCNT0 = 0; //Set Timer Initial value
	DDRB  = DDRB | (1<<PB3);
	//GPIO_setupPinDirection(DDRB,PB3,PIN_OUTPUT);//set PB3/OC0 as output pin --> pin where the PWM signal is generated from MC.
	/* Configure timer control register
	 * 1. Fast PWM mode FOC0=0
	 * 2. Fast PWM Mode WGM01=1 & WGM00=1
	 * 3. Clear OC0 when match occurs (non inverted mode) COM00=0 & COM01=1
	 * 4. clock = F_CPU/8 CS00=0 CS01=1 CS02=0
	 */
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) ;
}
static void PWM_Timer0_Start(uint8 duty_cycle)
{
	OCR0  = duty_cycle; // Set Compare Value
	TCCR0 |= (1<<CS01); //enable clock
}

void DCmotor_init(const DCmotor_configtype *motor)
{
	//PWM_Timer0_Init();
	GPIO_setupPinDirection(motor->port_num, motor->input_pin_1,PIN_OUTPUT);
	GPIO_setupPinDirection(motor->port_num, motor->input_pin_2,PIN_OUTPUT);
	GPIO_writePin(motor->port_num,motor->input_pin_1,LOGIC_LOW);  //input A
	GPIO_writePin(motor->port_num,motor->input_pin_2,LOGIC_LOW);  //input B
}

void DCmotor_Rotate(DCmotor_configtype *motor,DCMotor_state State,uint8 speed)
{
	//PWM_Timer0_Start(((uint16)(speed*255)/(100)));   //adjust motor speed
	if(State==CW)				//rotate clockwise
	{
		GPIO_writePin(motor->port_num,motor->input_pin_1,LOGIC_LOW);  //input A
		GPIO_writePin(motor->port_num,motor->input_pin_2,LOGIC_HIGH);  //input B
	}
	else if(State==ACW)			//rotate anti clockwise
	{
		GPIO_writePin(motor->port_num,motor->input_pin_1,LOGIC_HIGH);  //input A
		GPIO_writePin(motor->port_num,motor->input_pin_2,LOGIC_LOW);  //input B
	}
	else						//stop the motor
	{
		GPIO_writePin(motor->port_num,motor->input_pin_1,LOGIC_LOW);  //input A
		GPIO_writePin(motor->port_num,motor->input_pin_2,LOGIC_LOW);  //input B
	}

}
