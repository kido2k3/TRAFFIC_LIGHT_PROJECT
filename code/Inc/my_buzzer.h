/*
 * my_buzzer.h
 *
 *  Created on: Nov 23, 2023
 *      Author: atfox
 */

#ifndef INC_MY_BUZZER_H_
#define INC_MY_BUZZER_H_

#include<stdint.h>
#include "my_define.h"

#define NUMBER_OF_BUZZER            1

#define FREQUENCY_OF_TIM3           1000            
/*          Buzzer parameter            */
// Below unit <ms>
#define BUZZER_CHANGE_STATE_PERIOD  1000	/* = PEDESTRIAN_TIMER / 5(state) */
#define BUZZER_SLOW_PERIOD          500
#define BUZZER_ALMOSTSLOW_PERIOD    250
#define BUZZER_MEDIUM_PERIOD        125
#define BUZZER_ALMOSTFAST_PERIOD    60
#define BUZZER_FAST_PERIOD          30
// Below unit <%>
#define BUZZER_SLOW_DUTYCYCLE       20
#define BUZZER_ALMOSTSLOW_DUTYCYCLE 40
#define BUZZER_MEDIUM_DUTYCYCLE     50
#define BUZZER_ALMOSTFAST_DUTYCYCLE 70
#define BUZZER_FAST_DUTYCYCLE       90


void buzzer_init(void);                 /* Initial FSM */

void buzzer_processing(uint8_t index); /* Place in while(1) */

void buzzer_turnOn(uint8_t index);      /* Call in Traffic-Light FSM */
void buzzer_turnOff(uint8_t index);     /* Call in Traffic-Light FSM */

void timerBuzzerInit();                 /* Initial Timer */

int timer_counter_buzzer[NUMBER_OF_BUZZER];   /* INIT VALUE: setTimerBuzzer(0, (BUZZER_SLOW_PERIOD * ((BUZZER_SLOW_PERIOD / 100.0))) */
int timer_flag_buzzer[NUMBER_OF_BUZZER];
int timer_counter_buzzerState[NUMBER_OF_BUZZER];   /* INIT VALUE: setTimerBuzzerState(0, BUZZER_CHANGE_STATE_PERIOD) */
int timer_flag_buzzerState[NUMBER_OF_BUZZER];
void timerBuzzerRun(void);              /* Timer run */

#endif /* INC_MY_BUZZER_H_ */
