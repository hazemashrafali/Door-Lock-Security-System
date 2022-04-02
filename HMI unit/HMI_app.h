/*
 * HMI_app.h
 *
 *  Created on: Mar 25, 2022
 *      Author: HAZEM-PC
 */

#ifndef HMI_APP_H_
#define HMI_APP_H_

#define MCU_READY 0xFF

typedef enum{
	pass_equal,pass_short,pass_long
}password_state;

typedef enum{
	false,true
}system_state;

typedef enum{
	wrong_select,open_door,change_password
}menu_state;

#endif /* HMI_APP_H_ */
