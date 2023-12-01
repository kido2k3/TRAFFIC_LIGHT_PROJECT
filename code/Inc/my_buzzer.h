/*
 * my_buzzer.h
 *
 *  Created on: Dec 1, 2023
 *      Author: atfox
 */

#ifndef INC_MY_BUZZER_H_
#define INC_MY_BUZZER_H_

#include "my_define.h"

#define BUZZER_MODE_AMOUNT 		5

#define BUZZER_INTENSITY_MAX	100
#define BUZZER_INTENSITY_MIN	0
#define BUZZER_INTENSITY_MDF	((BUZZER_INTENSITY_MAX - BUZZER_INTENSITY_MIN) / BUZZER_MODE_AMOUNT)

// Param: None
// Proc : Start PWM of Channel_1
void buzzer_init(void);

// Param: None
// Proc : Set current_intensity to PWM_pin
void buzzer_on(void);

// Param: None
// Proc : Set LOW to PWM_pin (this function do not modify current_intensity)
void buzzer_off(void);

// Param: mode_range in [0:BUZZER_MODE_AMOUNT - 1]
// Proc : Set current_intensity = mode * BUZZER_INTENSITY_MDF
void buzzer_calculation(uint8_t mode);

#endif /* INC_MY_BUZZER_H_ */
