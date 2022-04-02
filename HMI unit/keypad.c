/*
 * keypad.c
 *
 *  Created on: Mar 7, 2022
 *      Author: HAZEM-PC
 */


#include "common_macros.h"
#include "keypad.h"
#include "gpio.h"
#include "util/delay.h"

uint8 KEYPAD_getPressedKey(Keypad *key)
{
	uint8 col,row,value;
	while(1)
	{
		for(col=0; col<key->col_no ;col++) /* loop for columns */
		{
			keypad_setport_direction(&key,PIN_INPUT);
			keypad_setcolpin_direction(&key,col,PIN_OUTPUT);
#if(KEYPAD_BUTTON_PRESSED == LOGIC_LOW)
			keypad_writeport(&key,1);
			keypad_writepin(&key,col,0);
#else
			keypad_writepin(&key,col,1);
			keypad_writeport(&key,0);
#endif
			for(row=0; row<key->row_no ;row++)
			{
				if(keypad_readpin(&key,row) == KEYPAD_BUTTON_PRESSED)
				{
					value= ((row*(key->col_no))+col+1);
					return key->keypad[value-1];
				}
			}
		}
	}
}

void keypad_setport_direction(Keypad **key,GPIO_PinDirectionType state)
{
	int x;
	for(x=0;x<(*key)->col_no;x++)
	{
		if((*key)->colpins[x] >= 0 && (*key)->colpins[x] <= 7)
		{
			GPIO_setupPinDirection(PORTA_ID,(*key)->colpins[x],state);
		}
		else if((*key)->colpins[x] >= 8 && (*key)->colpins[x] <= 15)
		{
			GPIO_setupPinDirection(PORTB_ID,(*key)->colpins[x]-8,state);
		}
		else if((*key)->colpins[x] >= 16 && (*key)->colpins[x] <= 23)
		{
			GPIO_setupPinDirection(PORTC_ID,(*key)->colpins[x]-16,state);
		}
		else if((*key)->colpins[x] >= 24 && (*key)->colpins[x] <= 32)
		{
			GPIO_setupPinDirection(PORTD_ID,(*key)->colpins[x]-24,state);
		}
	}
	for(x=0;x<(*key)->row_no;x++)
		{
			if((*key)->rowpins[x] >= 0 && (*key)->rowpins[x] <= 7)
			{
				GPIO_setupPinDirection(PORTA_ID,(*key)->rowpins[x],state);
			}
			else if((*key)->rowpins[x] >= 8 && (*key)->rowpins[x] <= 15)
			{
				GPIO_setupPinDirection(PORTB_ID,(*key)->rowpins[x]-8,state);
			}
			else if((*key)->rowpins[x] >= 16 && (*key)->rowpins[x] <= 23)
			{
				GPIO_setupPinDirection(PORTC_ID,(*key)->rowpins[x]-16,state);
			}
			else if((*key)->rowpins[x] >= 24 && (*key)->rowpins[x] <= 32)
			{
				GPIO_setupPinDirection(PORTD_ID,(*key)->rowpins[x]-24,state);
			}
		}
}

void keypad_setcolpin_direction(Keypad **key,uint8 index,GPIO_PinDirectionType state)
{
	if((*key)->colpins[index] >= 0 && (*key)->colpins[index] <=7)
	{
		GPIO_setupPinDirection(PORTA_ID,(*key)->colpins[index],state);
	}
	else if((*key)->colpins[index] >= 8 && (*key)->colpins[index] <=15)
	{
		GPIO_setupPinDirection(PORTB_ID,(*key)->colpins[index]-8,state);
	}
	else if((*key)->colpins[index] >= 16 && (*key)->colpins[index] <=23)
	{
		GPIO_setupPinDirection(PORTC_ID,(*key)->colpins[index]-16,state);
	}
	else if((*key)->colpins[index] >= 24 && (*key)->colpins[index] <=32)
	{
		GPIO_setupPinDirection(PORTD_ID,(*key)->colpins[index]-24,state);
	}
}

void keypad_setrowpin_direction(Keypad **key,uint8 index,GPIO_PinDirectionType state)
{
	if((*key)->rowpins[index] >= 0 && (*key)->rowpins[index] <=7)
	{
		GPIO_setupPinDirection(PORTA_ID,(*key)->rowpins[index],state);
	}
	else if((*key)->rowpins[index] >= 8 && (*key)->rowpins[index] <=15)
	{
		GPIO_setupPinDirection(PORTB_ID,(*key)->colpins[index]-8,state);
	}
	else if((*key)->rowpins[index] >= 16 && (*key)->rowpins[index] <=23)
	{
		GPIO_setupPinDirection(PORTC_ID,(*key)->colpins[index]-16,state);
	}
	else if((*key)->rowpins[index] >= 24 && (*key)->rowpins[index] <=32)
	{
		GPIO_setupPinDirection(PORTD_ID,(*key)->rowpins[index]-24,state);
	}
}

void keypad_writeport(Keypad **key,uint8 value)
{
	int x=0;
	for(x=0;x<(*key)->col_no;x++)
	{
		if((*key)->colpins[x] >= 0 && (*key)->colpins[x] <= 7)
		{
			GPIO_writePin(PORTA_ID,(*key)->colpins[x],value);
		}
		else if((*key)->colpins[x] >= 8 && (*key)->colpins[x] <= 15)
		{
			GPIO_writePin(PORTB_ID,(*key)->colpins[x]-8,value);
		}
		else if((*key)->colpins[x] >= 16 && (*key)->colpins[x] <= 23)
		{
			GPIO_writePin(PORTC_ID,(*key)->colpins[x]-16,value);
		}
		else if((*key)->colpins[x] >= 24 && (*key)->colpins[x] <= 32)
		{
			GPIO_writePin(PORTD_ID,(*key)->colpins[x]-24,value);
		}
	}
	for(x=0;x<(*key)->row_no;x++)
	{
		if((*key)->rowpins[x] >= 0 && (*key)->rowpins[x] <= 7)
		{
			GPIO_writePin(PORTA_ID,(*key)->rowpins[x],value);
		}
		else if((*key)->rowpins[x] >= 8 && (*key)->rowpins[x] <= 15)
		{
			GPIO_writePin(PORTB_ID,(*key)->rowpins[x]-8,value);
		}
		else if((*key)->rowpins[x] >= 16 && (*key)->rowpins[x] <= 23)
		{
			GPIO_writePin(PORTC_ID,(*key)->rowpins[x]-16,value);
		}
		else if((*key)->rowpins[x] >= 24 && (*key)->rowpins[x] <= 32)
		{
			GPIO_writePin(PORTD_ID,(*key)->rowpins[x]-24,value);
		}
	}
}
void keypad_writepin(Keypad **key,uint8 index,uint8 value)
{
	if((*key)->colpins[index] >= 0 && (*key)->colpins[index] <= 7)
	{
		GPIO_writePin(PORTA_ID,(*key)->colpins[index],value);
	}
	else if((*key)->colpins[index] >= 8 && (*key)->colpins[index] <= 15)
	{
		GPIO_writePin(PORTB_ID,(*key)->colpins[index]-8,value);
	}
	else if((*key)->colpins[index] >= 16 && (*key)->colpins[index] <= 23)
	{
		GPIO_writePin(PORTC_ID,(*key)->colpins[index]-16,value);
	}
	else if((*key)->colpins[index] >= 24 && (*key)->colpins[index] <= 32)
	{
		GPIO_writePin(PORTD_ID,(*key)->colpins[index]-24,value);
	}
}




uint8 keypad_readpin(Keypad **key,uint8 value)
{
	uint8 press;
				if((*key)->rowpins[value] >= 0 && (*key)->rowpins[value] <= 7)
				{
					press=GPIO_readPin(PORTA_ID,(*key)->rowpins[value]);
				}
				else if((*key)->rowpins[value] >= 8 && (*key)->rowpins[value] <= 15)
				{
					press=GPIO_readPin(PORTB_ID,(*key)->rowpins[value]-8);
				}
				else if((*key)->rowpins[value] >= 16 && (*key)->rowpins[value] <= 23)
				{
					press=GPIO_readPin(PORTC_ID,(*key)->rowpins[value]-16);
				}
				else if((*key)->rowpins[value] >= 24 && (*key)->rowpins[value] <= 32)
				{
					press=GPIO_readPin(PORTD_ID,(*key)->rowpins[value]-24);
				}
	return press;
}

