/*
 * my_system.h
 *
 *  Created on: Nov 18, 2023
 *      Author: HP
 */

#ifndef INC_MY_SYSTEM_H_
#define INC_MY_SYSTEM_H_

#include"my_define.h"
#include"my_scheduler.h"
#include"my_button.h"
extern TIM_HandleTypeDef htim2;
void init(void);
void loop(void);
#endif /* INC_MY_SYSTEM_H_ */
