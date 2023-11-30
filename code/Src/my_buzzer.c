/*
 * my_buzzer.c
 *
 *  Created on: Nov 27, 2023
 *      Author: atfox
 */

#include "my_buzzer.h"

enum BuzzerState {
	OFF_STATE,
	ALARM_STATE
};
struct BuzzerPhaseInfo {
	uint8_t  phaseState;		// On or Off
	uint32_t phasePeriod;	// Period of Phase
};
struct BuzzerStruct {
	enum BuzzerState buzzerState;
	uint8_t intensity;
	struct BuzzerPhaseInfo phaseInfo;

	TIM_HandleTypeDef *htim;
	int channel_address;

};
struct BuzzerStruct buzzer[NUMBER_OF_BUZZER];

void buzzer_init(void) {
	for(int i = 0; i < NUMBER_OF_BUZZER; i++) {
		buzzer[i].buzzerState = OFF_STATE;
		buzzer[i].intensity = 0;
		buzzer[i].phaseInfo.phaseState 	= 0;
		buzzer[i].phaseInfo.phasePeriod = BUZZER_PHASE_PERIOD_MAX;
		// Configuration of User
		switch(i) {
		case 0:
			buzzer[i].htim = &htim3;
			buzzer[i].channel_address = TIM_CHANNEL_1;
			break;
		case 1:
			break;
		case 2:
			break;
		}
		// Set up PWM_TIM
		__HAL_TIM_SetCompare(buzzer[i].htim, buzzer[i].channel_address, 100);
		timerFlagBuzzerChangeIntensity[i] = 0;
		timerFlagBuzzerPhase[i] = 0;
	}
}

/*
 * @brief:	Set Buzzer's intensity
 * @para:	<intensity> is number of buzzer's intensity (intensity == 0 is turning off buzzer) [0;100]
 * @retval:	none
 * */
void set_buzzer_intensity(uint8_t buzzer_index, uint8_t intensity) {
	if(intensity > 95) {
		__HAL_TIM_SetCompare(buzzer[buzzer_index].htim, buzzer[buzzer_index].channel_address, 0);
	}
	else if (intensity < 5) {
		__HAL_TIM_SetCompare(buzzer[buzzer_index].htim, buzzer[buzzer_index].channel_address, 100);
	}
	else {
		__HAL_TIM_SetCompare(buzzer[buzzer_index].htim, buzzer[buzzer_index].channel_address, 100 - intensity);
	}

}

void ChangeIntensityOneShot(void) {
	timerFlagBuzzerChangeIntensity[0] = 1;
}
void ChangePhaseOneShot(void) {
	timerFlagBuzzerPhase[0] = 1;
}

/*
 * @brief:	Turn on buzzer
 * @para:	Index of buzzer
 * @retval:	none
 * */
void buzzer_turn_on(uint8_t buzzer_index) {
	buzzer[buzzer_index].buzzerState 			= ALARM_STATE;
	buzzer[buzzer_index].intensity 				= BUZZER_INTENSITY_MIN;
	buzzer[buzzer_index].phaseInfo.phaseState 	= 1;
	buzzer[buzzer_index].phaseInfo.phasePeriod 	= BUZZER_PHASE_PERIOD_MAX;
	set_buzzer_intensity(buzzer_index, buzzer[buzzer_index].intensity);
	// Reset timer counter

#ifdef FUNCTION_TESTING
	set_timer_buzzer_change_intensity(buzzer_index, BUZZER_INTENSITY_CHANGE_TIMER);
	set_timer_buzzer_phase(buzzer_index, buzzer[buzzer_index].phaseInfo.phasePeriod);
#else
	timerFlagBuzzerChangeIntensity[buzzer_index] = 0;
	timerFlagBuzzerPhase[buzzer_index] = 0;
	sch_add_task(ChangeIntensityOneShot, BUZZER_INTENSITY_CHANGE_TIMER, 0);
	sch_add_task(ChangePhaseOneShot, buzzer[buzzer_index].phaseInfo.phasePeriod, 0);
#endif
}

void buzzer_turn_off(uint8_t buzzer_index) {
	buzzer[buzzer_index].buzzerState 			= OFF_STATE;
	buzzer[buzzer_index].intensity 				= 0;
	buzzer[buzzer_index].phaseInfo.phaseState 	= 0;
	buzzer[buzzer_index].phaseInfo.phasePeriod 	= BUZZER_PHASE_PERIOD_MAX;
	set_buzzer_intensity(buzzer_index, buzzer[buzzer_index].intensity);
	// Reset timer counter
#ifdef FUNCTION_TESTING
	set_timer_buzzer_change_intensity(buzzer_index, BUZZER_INTENSITY_CHANGE_TIMER);
	set_timer_buzzer_phase(buzzer_index, buzzer[buzzer_index].phaseInfo.phasePeriod);
#else
	timerFlagBuzzerChangeIntensity[buzzer_index] = 0;
	timerFlagBuzzerPhase[buzzer_index] = 0;
#endif
}

/*
 * @brief:	Get information
 * @para:	Index of buzzer
 * @retval:	Buzzer is alarm
 * */
int is_buzzer_alarm(uint8_t buzzer_index) {
	return (buzzer[buzzer_index].buzzerState == ALARM_STATE);
}

/*
 * @brief:	Run FSM
 * @para:	Index of buzzer
 * @retval:	none
 * */
void buzzer0_fsm(void) {
	int buzzer_index = 0;
	switch(buzzer[buzzer_index].buzzerState) {
	case OFF_STATE:
		break;
	case ALARM_STATE:
		if(timerFlagBuzzerChangeIntensity[buzzer_index]) {
			// Modify information
			buzzer[buzzer_index].intensity 				= (buzzer[buzzer_index].intensity + BUZZER_INTENSITY_MODIFY_VALUE);
			buzzer[buzzer_index].phaseInfo.phaseState 	= 1;
			buzzer[buzzer_index].phaseInfo.phasePeriod 	= buzzer[buzzer_index].phaseInfo.phasePeriod - BUZZER_PHASE_PERIOD_MODIFY_VALUE;
			// Set PWM
			set_buzzer_intensity(buzzer_index, buzzer[buzzer_index].intensity);
			// Set timer
#ifdef FUNCTION_TESTING
			set_timer_buzzer_change_intensity(buzzer_index, BUZZER_INTENSITY_CHANGE_TIMER);
#else
			timerFlagBuzzerChangeIntensity[buzzer_index] = 0;
			sch_add_task(ChangeIntensityOneShot, BUZZER_INTENSITY_CHANGE_TIMER, 0);
#endif
		}
		if(timerFlagBuzzerPhase[buzzer_index]) {
#ifdef FUNCTION_TESTING
			set_timer_buzzer_phase(buzzer_index, buzzer[buzzer_index].phaseInfo.phasePeriod);
#else
			timerFlagBuzzerPhase[buzzer_index] = 0;
			sch_add_task(ChangePhaseOneShot, buzzer[buzzer_index].phaseInfo.phasePeriod, 0);
#endif
			if(buzzer[buzzer_index].phaseInfo.phaseState) {
				set_buzzer_intensity(buzzer_index, 0);
				buzzer[buzzer_index].phaseInfo.phaseState  = 0;
			}
			else {
				set_buzzer_intensity(buzzer_index, buzzer[buzzer_index].intensity);
				buzzer[buzzer_index].phaseInfo.phaseState = 1;
			}
		}
		break;
	}
}



void set_timer_buzzer_change_intensity(uint8_t buzzer_index, int timer) {
	timerCounterBuzzerChangeIntensity[buzzer_index] = timer  * FREQUENCY_OF_TIM/1000;
	timerFlagBuzzerChangeIntensity[buzzer_index]	= 0;
}

void set_timer_buzzer_phase(uint8_t buzzer_index, int timer) {
	timerCounterBuzzerPhase[buzzer_index] = timer  * FREQUENCY_OF_TIM/1000;
	timerFlagBuzzerPhase[buzzer_index]	= 0;
}

void buzzer_timer_run(void) {

	// Processing Buzzer[0]
	if(is_buzzer_alarm(0)) {
		timerCounterBuzzerPhase[0]--;
		if(timerCounterBuzzerPhase[0] == 0) {
			timerFlagBuzzerPhase[0] = 1;
		}
		timerCounterBuzzerChangeIntensity[0]--;
		if(timerCounterBuzzerChangeIntensity[0] == 0) {
			timerFlagBuzzerChangeIntensity[0] = 1;
		}
	}

}



