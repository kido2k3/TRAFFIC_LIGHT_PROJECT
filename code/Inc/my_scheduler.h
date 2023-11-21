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

bool sch_add_task(void (*pTask)(), uint16_t delay, uint16_t period);
void sch_update(void);
bool sch_dispatch(void);
#endif /* INC_MY_SCHEDULER_H_ */
