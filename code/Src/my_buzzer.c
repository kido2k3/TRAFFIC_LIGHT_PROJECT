/*
 * buzzer.c
 *
 *  Created on: Dec 1, 2023
 *      Author: atfox
 */

#include "my_buzzer.h"

int current_intensity  = 0;
struct BuzzerStruct {
	int current_intensity;
	int current_toggle_time;
	TIM_HandleTypeDef *htim;
	int channel_address;

} buzzer;
// Param: None
// Proc : Start PWM of Channel_1
void buzzer_init(void) {
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	buzzer.current_intensity = BUZZER_INTENSITY_MIN;
	buzzer.current_toggle_time = BUZZER_TOGGLE_TIME_MDF;
	buzzer.htim = &htim3;
	buzzer.channel_address = TIM_CHANNEL_1;
	__HAL_TIM_SetCompare(buzzer.htim, buzzer.channel_address, BUZZER_INTENSITY_MIN);
}

// Param: None
// Proc : Set current_intensity to PWM_pin
void buzzer_on(void) {
	__HAL_TIM_SetCompare(buzzer.htim, buzzer.channel_address, buzzer.current_intensity);
}

// Param: None
// Proc : Set LOW to PWM_pin (this function do not modify current_intensity)
void buzzer_off(void) {
	__HAL_TIM_SetCompare(buzzer.htim, buzzer.channel_address, BUZZER_INTENSITY_MIN);
}

// Param: mode_range in [0:BUZZER_MODE_AMOUNT - 1]
// Proc : Set current_intensity = mode * BUZZER_INTENSITY_MDF
void buzzer_calculation(uint8_t mode) {
	if(mode == 0) {
		buzzer.current_intensity = BUZZER_INTENSITY_MIN;
		buzzer.current_toggle_time = BUZZER_TOGGLE_TIME_MDF;
	}
	else {
		buzzer.current_intensity = (BUZZER_MODE_AMOUNT - mode) * BUZZER_INTENSITY_MDF;
		buzzer.current_toggle_time = mode * BUZZER_TOGGLE_TIME_MDF;
	}
}

// Param : None
// Retval: Toggle_time (unit = 1ms)
int buzzer_getToggle_time (void) {
	return buzzer.current_toggle_time;
}


