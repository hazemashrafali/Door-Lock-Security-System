/*
 * HMI_app.c
 *
 *  Created on: Mar 23, 2022
 *      Author: HAZEM-PC
 */

#include "std_types.h"
#include "avr/interrupt.h"
#include "TIMER.h"
#include "keypad.h"
#include "lcd.h"
#include "UART.h"
#include "util/delay.h"
#include "HMI_app.h"

#define N 10
#define row 4
#define col 4

/**********FUNCTION PROTOTYPES*************/
password_state take_password(uint8 *ppass,uint8 size);
uint8 recieve_state(void);
uint8 read_key(void);
void display_menu(void);

/**********GLOBAL VARIABLES*************/
uint8 keypad[row][col]= {
		{'7','8','9','/'},
		{'4','5','6','*'},
		{'1','2','3','-'},
		{'_','0','=','+'}};
uint8 rowpin[row]={16,17,18,19};
uint8 colpin[col]={20,21,22,23};
Keypad key1={keypad,rowpin,colpin,row,col};


int main(void)
{
	uint8 x,key,unmatcing_count=0;
	menu_state key_state=wrong_select;
	uint8 pass_compare_state=FALSE;
	uint8 pass_flag;
	password_state pass_state=pass_short;
	uint8 pass[N];
	UART_configtype uart={bit_8,Disable,_1_bit,9600};
	UART_init(&uart);
	LCD_init();
	SET_BIT(SREG,7);
	pass_flag=recieve_state();

	while(1)
	{
		/*CODE TO HANDLE FIRST TIME SYSTEM RUNNING AND HANDLE IF PASSWORD WAS NOT SET BEFORE*/
		while((pass_flag != TRUE) && (pass_state != pass_equal) && (pass_compare_state==FALSE))
		{
			_delay_ms(100);
			LCD_clearScreen();
			LCD_displayString("enter pass of 5 char");
			LCD_moveCursor(1, 0);
			pass_state=take_password(pass,N);
			if(pass_state==pass_equal)
			{
				for(x=0;x<5;x++)
				{
					UART_sendByte(pass[x]);
				}
				_delay_ms(1000);
				LCD_clearScreen();
				LCD_displayString("re-enter password");
				LCD_moveCursor(1, 0);
				take_password(pass,N);
				for(x=0;x<5;x++)
				{
					UART_sendByte(pass[x]);
				}
			}
			else if(pass_state==pass_short)
			{
				LCD_clearScreen();
				LCD_displayStringRowColumn(0,2,"short password");
				_delay_ms(5000);
				continue;
			}
			else
			{
				LCD_clearScreen();
				LCD_displayStringRowColumn(0,2,"long password");
				_delay_ms(5000);
				continue;
			}
			pass_compare_state=recieve_state();
			if(pass_compare_state==FALSE)
			{
				LCD_clearScreen();
				LCD_displayString("not matching password");
				pass_state=pass_short;
				_delay_ms(5000);
				continue;
			}
			else
			{
				LCD_clearScreen();
				LCD_displayString("matching password");
				_delay_ms(5000);
			}
		}
		/*CODE TO HANDLE SYSTEM MAIN MENU*/
		display_menu();
		while(key_state == wrong_select)
		{
			key=read_key();
			if(key==1)
			{
				key_state = open_door;
				UART_sendByte(key);
				UART_sendByte('_');
			}
			else if(key==2)
			{
				key_state = change_password;
				UART_sendByte(key);
				UART_sendByte('_');
			}
			else
			{
				key_state = wrong_select;
			}
		}
		/*CODE TO HANDLE IF PASSWORD IS WRONG FOR 3 TIMES*/
		while(unmatcing_count<3)
		{
			if(key_state == open_door || key_state == change_password)
			{
				LCD_clearScreen();
				LCD_displayString("enter password");
				LCD_moveCursor(1, 0);
				take_password(pass, N);
				for(x=0;x<5;x++)
					UART_sendByte(pass[x]);
				pass_compare_state=recieve_state();
				if(pass_compare_state==false)
				{
					LCD_clearScreen();
					LCD_displayString("wrong password!");
					unmatcing_count++;
					if(unmatcing_count==3)
					{
						LCD_clearScreen();
						LCD_displayString("error message!");
						_delay_ms(1500);
						key_state=wrong_select;
					}
				}
				else
				{
					pass_compare_state=false;
					unmatcing_count=0;
					break;
				}
			}
		}
		/*CODE TO HANDLE DOOR OPENING SELECTION*/
		if(key_state==open_door)
		{
			LCD_clearScreen();
			LCD_displayString("door unlocked!");
			_delay_ms(15000);
			LCD_clearScreen();
			LCD_displayString("door locked!");
			_delay_ms(15000);
		}
		/*CODE TO HANDLE CHANGING OLD PASSWORD*/
		if(key_state==change_password)
		{
			while(pass_compare_state==false)
			{
				LCD_clearScreen();
				LCD_displayString("enter pass of 5 char");
				LCD_moveCursor(1, 0);
				pass_state=take_password(pass,N);   //take first pass
				if(pass_state==pass_equal)         //check pass length
				{
					for(x=0;x<5;x++)
					{
						UART_sendByte(pass[x]);     //send pass if length equal 5
					}
					LCD_clearScreen();
					LCD_displayString("re-enter password");
					LCD_moveCursor(1, 0);
					take_password(pass,N);       ///take second pass
					for(x=0;x<5;x++)
					{
						UART_sendByte(pass[x]);    //send pass if length equal 5
					}
				}
				else if(pass_state==pass_short)   //if pass length < 5
				{
					LCD_clearScreen();
					LCD_displayStringRowColumn(0,2,"short password");
					_delay_ms(1000);
				}
				else    							//if pass length > 5
				{
					LCD_clearScreen();
					LCD_displayStringRowColumn(0,2,"long password");
					_delay_ms(1000);
				}
				pass_compare_state=recieve_state();   //check pass matching
				if(pass_compare_state==false)		  //if pass not matching
				{
					LCD_clearScreen();
					LCD_displayString("not matching password");
					_delay_ms(1000);
				}
				else								//if pass matching
				{
					LCD_clearScreen();
					LCD_displayString("password set");
					_delay_ms(1000);
				}
			}
		}
	}
}





/*Description:function used to take password and check for size constrains */
password_state take_password(uint8 *ppass,uint8 size)
{
	uint8 key=0,count=0;
	password_state state;
	while((key != '_') && (size > count))
	{
		key=KEYPAD_getPressedKey(&key1);
		_delay_ms(800);
		if(key != '_')
		{
			ppass[count]=key;
			count++;
			_delay_ms(800);
			LCD_displayCharacter(key);
		}
		else
		{
			if(count == 5)
				state=pass_equal;
			else if(count > 5)
				state=pass_long;
			else
				state=pass_short;
		}
	}
	return state;
}

/*Description:function used to determine system state from control unit */
uint8 recieve_state(void)
{
	uint8 data;
	data = UART_recieveByte();
	if(data==1)
		return 1;
	else
		return 0;
}

/*Description:function used to display system main menu*/
void display_menu(void)
{
	LCD_clearScreen();
	LCD_displayString("+:open door");
	LCD_displayStringRowColumn(1,0,"-:change password");
}

/*Description:function used to indicate main menu selection */
uint8 read_key(void)
{
	uint8 key;
	uint8 state;
	key=KEYPAD_getPressedKey(&key1);
	_delay_ms(1000);
	if(key=='+')
	{
		state=1;
	}
	else if(key=='-')
	{
		state=2;
	}
	else
	{
		state=0;
	}
	return state;
}

