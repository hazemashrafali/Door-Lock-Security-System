/*
 * DCmotor.h
 *
 *  Created on: Feb 23, 2022
 *      Author: HAZEM-PC
 */

#ifndef DCMOTOR_H_
#define DCMOTOR_H_

#include"std_types.h"


typedef struct{
	uint8 port_num;
	uint8 input_pin_1;
	uint8 input_pin_2;
	uint8 Enable_pin;
}DCmotor_configtype;

typedef enum{
	stop,CW,ACW
}DCMotor_state;

void DCmotor_init(const DCmotor_configtype *motor);
void DCmotor_Rotate(DCmotor_configtype *motor,DCMotor_state State,uint8 speed);
void PWM_Timer0_Init(void);
#endif /* DCMOTOR_H_ */
