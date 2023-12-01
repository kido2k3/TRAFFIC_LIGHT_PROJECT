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
#include"my_led.h"
#include"my_fsm.h"
#include"my_uart.h"
#include"my_buzzer.h"

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
void init(void);
void loop(void);
#endif /* INC_MY_SYSTEM_H_ */
