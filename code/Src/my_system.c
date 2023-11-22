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

	sch_add_task(button_read, 0, 10);
}

void loop(void)
{
	sch_dispatch();
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == htim2.Instance)
	{
		sch_update();
	}
}
