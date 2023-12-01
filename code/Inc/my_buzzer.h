/*
 * my_buzzer.h
 *
 *  Created on: Nov 27, 2023
 *      Author: atfox
 */

#ifndef INC_MY_BUZZER_H_
#define INC_MY_BUZZER_H_

#include "my_define.h"
#include "my_system.h"
//#include "my_scheduler.h"

//#define FUNCTION_TESTING		// For single testing

#define NUMBER_OF_BUZZER 		1

#define FREQUENCY_OF_TIM					1000// Use timer2 to count Phase period

#define BUZZER_ON_DURATION					5000

#define BUZZER_INTENSITY_CHANGE_TIMER		1000// Unit <ms>
#define BUZZER_INTENSITY_CHANGE_STATE		(BUZZER_ON_DURATION / BUZZER_INTENSITY_CHANGE_TIMER)

#define BUZZER_INTENSITY_MAX 				95	// Base on duty-cycle
#define BUZZER_INTENSITY_MIN 				5	// Base on duty-cycle
#define BUZZER_INTENSITY_MODIFY_VALUE 		((BUZZER_INTENSITY_MAX - BUZZER_INTENSITY_MIN) / BUZZER_INTENSITY_CHANGE_STATE)

#define BUZZER_PHASE_PERIOD_MAX				400	// Unit <ms>
#define BUZZER_PHASE_PERIOD_MIN				20	// Unit <ms>
#define BUZZER_PHASE_PERIOD_MODIFY_VALUE 	((BUZZER_PHASE_PERIOD_MAX - BUZZER_PHASE_PERIOD_MIN) / BUZZER_INTENSITY_CHANGE_STATE)// Unit <ms>


void buzzer_init(void);
void buzzer_turn_on(uint8_t buzzer_index);
void buzzer_turn_off(uint8_t buzzer_index);
void buzzer0_fsm(void);

int timerCounterBuzzerPhase[NUMBER_OF_BUZZER];
int timerFlagBuzzerPhase[NUMBER_OF_BUZZER];
int timerCounterBuzzerChangeIntensity[NUMBER_OF_BUZZER];
int timerFlagBuzzerChangeIntensity[NUMBER_OF_BUZZER];
void set_timer_buzzer_change_intensity(uint8_t buzzer_index, int timer);
void set_timer_buzzer_phase(uint8_t buzzer_index, int timer);
void buzzer_timer_run(void);

#endif /* INC_MY_BUZZER_H_ */
