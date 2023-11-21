/*
 * button.h
 *
 *  Created on: Oct 30, 2023
 *      Author: HP
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include"port.h"
#include<stdint.h>



void button_read(void);
bool is_button_pressed(unsigned i);
bool is_button_long_pressed(unsigned i);
void button_init(void);

#endif /* INC_BUTTON_H_ */
