/*
 * keypad.h
 *
 *  Created on: Mar 7, 2022
 *      Author: HAZEM-PC
 */


#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "gpio.h"
#include "std_types.h"

typedef struct{
	uint8 *keypad;     //user defined keypad
	uint8 *rowpins;    //row pins array
	uint8 *colpins;    //col pins array
	uint8 row_no;      //no of rows
	uint8 col_no;      //no of cols
}Keypad;              //keypad structure configuration

#define KEYPAD_BUTTON_PRESSED            LOGIC_LOW
#define KEYPAD_BUTTON_RELEASED           LOGIC_HIGH


uint8 KEYPAD_getPressedKey(Keypad *key);
void keypad_setport_direction(Keypad **key,GPIO_PinDirectionType state);
void keypad_setcolpin_direction(Keypad **key,uint8 index,GPIO_PinDirectionType state);
void keypad_setrowpin_direction(Keypad **key,uint8 index,GPIO_PinDirectionType state);
void keypad_writeport(Keypad **key,uint8 value);
uint8 keypad_readpin(Keypad **key,uint8 value);
void keypad_writepin(Keypad **key,uint8 index,uint8 value);

#endif /* KEYPAD_H_ */
