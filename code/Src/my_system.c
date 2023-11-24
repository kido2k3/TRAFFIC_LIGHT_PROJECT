/*
 * my_system.c
 *
 *  Created on: Nov 18, 2023
 *      Author: HP
 */

#include "my_system.h"


void init(void)
{
	HAL_TIM_Base_Start_IT(&htim2);
	button_init();
	init_led();
	init_task();
}

void loop(void)
{
	sch_dispatch();
	fsm();
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == htim2.Instance)
	{
		sch_update();
	}
}
