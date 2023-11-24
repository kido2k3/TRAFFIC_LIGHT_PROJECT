/*
 * my_task.c
 *
 *  Created on: Nov 24, 2023
 *      Author: HP
 */

#include"my_task.h"
void task_send_data(void);
void init_task(void) {
	sch_add_task(button_read, 0, READ_BUTTON_TIME);
	sch_add_task(task_send_data, 0, ONE_SECOND);
}

/*
 * @brief:	check if scan 7 seg or not
 * @para:	none
 * @retval:	1 - successful
 * 			0 - fail
 * */
void task_send_data(void){
//	if(light_st == MANUALLY_SET || light_st == SWITCH_TRAFFIC_STATE){
//		off_all7led();
//	}
//	else if (light_st != TRAFFIC_LIGHT || red_time == green_time + yellow_time)
//	{
//		scan7SEG();
//	}
//	else
//	{
//		off_all7led();
//	}
}
