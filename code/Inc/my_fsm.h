/*
 * fsm.h
 *
 *  Created on: Oct 30, 2023
 *      Author: HP
 */

#ifndef INC_MY_FSM_H_
#define INC_MY_FSM_H_

#include <my_button.h>
#include <my_led.h>
#include "my_define.h"
#include "my_scheduler.h"
#include "my_uart.h"

void fsm(void);
void task_scan7seg(void);
#endif /* INC_MY_FSM_H_ */
