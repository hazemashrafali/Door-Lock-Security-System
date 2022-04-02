/*
 * Buzzer.c
 *
 *  Created on: Mar 24, 2022
 *      Author: HAZEM-PC
 */


#include "avr/io.h"
#include "Buzzer.h"

void Buzzer_init(const Buzzer_configtype *buzzer)
{
	GPIO_setupPinDirection(buzzer->port_num,buzzer->pin_num,buzzer->pin_dir);
	GPIO_writePin(buzzer->port_num,buzzer->pin_num,buzzer->pin_value);
}

void Buzzer_set(const Buzzer_configtype *buzzer,Buzzer_state state)
{
	if(state==buzzer_on)
	{
		GPIO_writePin(buzzer->port_num,buzzer->pin_num,LOGIC_HIGH);
	}
	else
	{
		GPIO_writePin(buzzer->port_num,buzzer->pin_num,LOGIC_LOW);
	}
}
