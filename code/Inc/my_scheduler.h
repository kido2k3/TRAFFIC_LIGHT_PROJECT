/*
 * scheduler.h
 *
 *  Created on: Nov 18, 2023
 *      Author: HP
 */

#ifndef INC_MY_SCHEDULER_H_
#define INC_MY_SCHEDULER_H_

#include"my_define.h"
#include<stdlib.h>

#define FREQUENCY_OF_TIM	1000 //Hz
#define READ_BUTTON_TIME	10 //10ms
#define SCAN_7SEGLED_TIME	50 //50ms
#define ONE_SECOND			1000//1s
#define TOGGLE_TIME			250
#define INCREASE_TIME		250

bool sch_add_task(void (*pTask)(), uint16_t delay, uint16_t period);
void sch_update(void);
bool sch_dispatch(void);
#endif /* INC_MY_SCHEDULER_H_ */
