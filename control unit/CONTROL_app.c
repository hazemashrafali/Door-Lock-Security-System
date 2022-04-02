/*
 * CONTROL_app.c
 *
 *  Created on: Mar 25, 2022
 *      Author: HAZEM-PC
 */


#include "CONTROL_app.h"
#include "std_types.h"
#include "avr/interrupt.h"
#include "TIMER.h"
#include "UART.h"
#include "DCmotor.h"
#include "Buzzer.h"
#include "external_eeprom.h"
#include "util/delay.h"



/**********FUNCTION PROTOTYPES*************/
void set_pass_flag(uint8 flag); 											//function used to save password state in EEPROM
uint8 read_pass_flag(void);  												//function used to read password state in EEPROM
void take_password(uint8 *pass,uint8 length); 								//function to save password in array
uint8 compare_password(uint8 *pass_1,uint8 *pass_2,uint8 length);			//function compare between first and second password
void write_password(uint8 *pass,uint8 length);								//function to save password in EEPROM
void read_password(uint8 *pass,uint8 length);								//function to read password from EEPROM
password_state change_password(uint8 *pass_1,uint8 *pass_2,uint8 length);	//function to change old password saved in EEPROM
menu_state read_key(void);													//function to read menu choice key

/**********GLOBAL VARIABLES*************/
uint8 pass_1[PASS_LENGTH],pass_2[PASS_LENGTH];


int main(void)
{
	uint8 count=0;
	menu_state key_choice;
	uint8 pass_flag;
	password_state pass_comp=not_match;
	UART_configtype uart={bit_8,Disable,_1_bit,9600};
	UART_init(&uart);
	Buzzer_configtype buzzer={PORTC_ID,PIN4_ID,PIN_OUTPUT,LOGIC_LOW};
	Buzzer_init(&buzzer);
	DCmotor_configtype motor={PORTC_ID,PIN5_ID,PIN6_ID,LOGIC_LOW};
	DCmotor_init(&motor);
	SET_BIT(SREG,7);
	EEPROM_init();
	pass_flag=read_pass_flag();
	UART_sendByte(pass_flag);

	while(1)
	{
		while(read_pass_flag()==FALSE || pass_comp != match)
		{
			take_password(pass_1,PASS_LENGTH);
			take_password(pass_2,PASS_LENGTH);
			pass_comp=compare_password(pass_1, pass_2, PASS_LENGTH);
			UART_sendByte(pass_comp);
			if(pass_comp==match)
				write_password(pass_1,PASS_LENGTH);
		}
		key_choice=read_key();
		while(count <3)
		{
			if(key_choice==open_door || key_choice==change_pass)
			{
				take_password(pass_1,PASS_LENGTH);
				read_password(pass_2,PASS_LENGTH);
				pass_comp=compare_password(pass_1, pass_2, PASS_LENGTH);
				UART_sendByte(pass_comp);
				if(pass_comp==false)
				{
					count++;
				}
				if(count==3)
				{
					Buzzer_set(&buzzer, buzzer_on);
					_delay_ms(1500);
				}
			}
			else
			{
				break;
			}
		}
		if(pass_comp==true && key_choice==open_door)
		{
			DCmotor_Rotate(&motor, CW, 255);
			_delay_ms(15000);
			DCmotor_Rotate(&motor, ACW, 255);
			_delay_ms(15000);
		}
		if(pass_comp==true && key_choice==change_pass)
		{
			take_password(pass_1,PASS_LENGTH);
			take_password(pass_2,PASS_LENGTH);
			pass_comp=compare_password(pass_1, pass_2, PASS_LENGTH);
			UART_sendByte(pass_comp);
			if(pass_comp==true)
				write_password(pass_1,PASS_LENGTH);
		}
	}
}



void set_pass_flag(uint8 flag)
{
	EEPROM_writeByte(FLAG_ADD,flag);
}

uint8 read_pass_flag(void)
{
	uint8 flag;
	EEPROM_readByte(FLAG_ADD,&flag);
	if(flag!=1)
		return 0;
	else
		return 1;
}

void take_password(uint8 *pass,uint8 length)
{
	uint8 x=0;
	while(x<length)
	{
		pass[x]=UART_recieveByte();
		x++;
	}
}

uint8 compare_password(uint8 *pass_1,uint8 *pass_2,uint8 length)
{
	uint8 x=0;
	uint8 flag=1;
	while(x<length)
	{
		if(pass_1[x] != pass_2[x])
		{
			flag=0;
		}
		x++;
	}
	return flag;
}

void write_password(uint8 *pass,uint8 length)
{
	uint8 x;
	for(x=0;x<length;x++)
	{
		EEPROM_writeByte((PASS_ADD)+x, pass[x]);
	}
	set_pass_flag(TRUE);
}

void read_password(uint8 *pass,uint8 length)
{
	uint8 x;
	for(x=0;x<length;x++)
	{
		EEPROM_readByte((PASS_ADD)+x, &pass[x]);
	}
}

password_state change_password(uint8 *pass_1,uint8 *pass_2,uint8 length)
{
	password_state state;
	take_password(pass_1, length);
	take_password(pass_2, length);
	state=compare_password(pass_1, pass_2, length);
	if(state==match)
		write_password(pass_1, length);
	return state;
}

menu_state read_key(void)
{
	uint8 key1,key2;
	key1=UART_recieveByte();
	key2=UART_recieveByte();
	if(key1==1 && key2=='_')
		return open_door;
	else if(key1==2 && key2=='_')
		return change_pass;
	else if(key1==0 && key2=='_')
		return wrong;
	else
		return 4;
}
