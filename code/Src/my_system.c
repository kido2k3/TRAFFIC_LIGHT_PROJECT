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
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	button_init();
	buzzer_init();
	init_led();
	uart_Init();
	sch_add_task(button_read, 0, READ_BUTTON_TIME);
}

void loop(void)
{
	sch_dispatch();
	fsm();
}
int counter = 0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == htim2.Instance)
	{
		sch_update();
	}
}
