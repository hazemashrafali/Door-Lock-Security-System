/*
 * CONTROL_app.h
 *
 *  Created on: Mar 25, 2022
 *      Author: HAZEM-PC
 */

#ifndef CONTROL_APP_H_
#define CONTROL_APP_H_


typedef enum{
	not_match,match
}password_state;

typedef enum{
	false,true
}state;

typedef enum{
	open_door,change_pass,wrong
}menu_state;

#define MCU_READY 0xFF
#define PASS_ADD 0x0100
#define FLAG_ADD 0x0300
#define PASS_LENGTH 5

#endif /* CONTROL_APP_H_ */
