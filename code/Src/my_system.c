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
	uart_Init();
	buzzer_init();
	sch_add_task(button_read, 0, READ_BUTTON_TIME);
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
