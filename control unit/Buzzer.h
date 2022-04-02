/*
 * Buzzer.h
 *
 *  Created on: Mar 24, 2022
 *      Author: HAZEM-PC
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#include"std_types.h"
#include "gpio.h"

typedef struct{
	uint8 port_num;
	uint8 pin_num;
	uint8 pin_dir;
	uint8 pin_value;
}Buzzer_configtype;

typedef enum{
	buzzer_off,buzzer_on
}Buzzer_state;

void Buzzer_init(const Buzzer_configtype *buzzer);
void Buzzer_set(const Buzzer_configtype *buzzer,Buzzer_state state);

#endif /* BUZZER_H_ */
