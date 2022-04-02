/*
 * UART.h
 *
 *  Created on: Mar 15, 2022
 *      Author: HAZEM-PC
 */

#ifndef UART_H_
#define UART_H_

/*******************************************************************************
 *                               file include                                  *
 *******************************************************************************/
#include "std_types.h"

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/
typedef enum{
	Disable,Even_Parity=2,Odd_Parity
}parity_mode;

typedef enum{
	_1_bit,_2_bit
}stop_bit;

typedef enum{
	bit_5,bit_6,bit_7,bit_8,bit_9=7
}bit_size;

typedef struct{
	bit_size No_of_bits;
	parity_mode partity_bit;
	stop_bit No_of_stop_bit;
	uint32 baud_rate;
}UART_configtype;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(UART_configtype *frame);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str);

#endif /* UART_H_ */
