/*
 * my_buzzer.c
 *
 *  Created on: Nov 23, 2023
 *      Author: atfox
 */


#include <my_buzzer.h>



enum BuzzerOnState {
    SLOW,
    ALMOSTSLOW,
    MEDIUM,
    ALMOSTFAST,
    FAST
};

struct {
    int is_on;
	enum BuzzerOnState onState;

	GPIO_TypeDef *port;
	uint16_t pin;
} buzzer[NUMBER_OF_BUZZER];



void buzzer_init(void) {
    for(int i = 0; i < NUMBER_OF_BUZZER; i++) {
        buzzer[i].is_on = 0;
        buzzer[i].onState = SLOW;
        setTimerBuzzer(i, BUZZER_SLOW_PERIOD * ((BUZZER_SLOW_PERIOD / 100.0)));
        setTimerBuzzerState(i, BUZZER_CHANGE_STATE_PERIOD);
        // Hard configuration
        switch(i) {
        case 0:
            buzzer[i].port = BUZZER_0_GPIO_Port;
            buzzer[i].pin  = BUZZER_0_Pin;
            break;
        case 1:
            break;
       }
    }
    timerBuzzerInit();
}

// Return ring time or software timer
// Arg: <curState: current state of buzzer after state is updated> <pulseHigh: pulse of buzzer is HIGH>
//              <For Period>                                                <For DutyCycle>
int getNetxRingTime(enum BuzzerOnState curState, int pulseHigh) {
    switch(curState) {
        case SLOW:
            if(pulseHigh) return (BUZZER_SLOW_PERIOD * (1 - (BUZZER_SLOW_DUTYCYCLE / 100.0)));
            else return (BUZZER_SLOW_PERIOD * ((BUZZER_SLOW_DUTYCYCLE / 100.0)));
            break;
        case ALMOSTSLOW:
            if(pulseHigh) return (BUZZER_ALMOSTSLOW_PERIOD * (1 - (BUZZER_ALMOSTSLOW_DUTYCYCLE / 100.0)));
            else return (BUZZER_ALMOSTSLOW_PERIOD * ((BUZZER_ALMOSTSLOW_DUTYCYCLE / 100.0)));
            break;
        case MEDIUM:
            if(pulseHigh) {
            	return (BUZZER_MEDIUM_PERIOD * (1 - (BUZZER_MEDIUM_DUTYCYCLE / 100.0)));
            }
            else {
            	return (BUZZER_MEDIUM_PERIOD * ((BUZZER_MEDIUM_DUTYCYCLE / 100.0)));
            }
            break;
        case ALMOSTFAST:
            if(pulseHigh) return (BUZZER_ALMOSTFAST_PERIOD * (1 - (BUZZER_ALMOSTFAST_DUTYCYCLE / 100.0)));
            else return (BUZZER_ALMOSTFAST_PERIOD * ((BUZZER_ALMOSTFAST_PERIOD / 100.0)));
            break;
        case FAST:
            if(pulseHigh) return (BUZZER_FAST_PERIOD * (1 - (BUZZER_FAST_DUTYCYCLE / 100.0)));
            else return (BUZZER_FAST_PERIOD * ((BUZZER_FAST_PERIOD / 100.0)));
            break;
    }
}

void buzzer_resetState(uint8_t index) {
    buzzer[index].is_on = 0;
    buzzer[index].onState = SLOW;
    setTimerBuzzer(index, BUZZER_SLOW_PERIOD * ((BUZZER_SLOW_PERIOD / 100.0)));
    setTimerBuzzerState(index, BUZZER_CHANGE_STATE_PERIOD);
    HAL_GPIO_WritePin(buzzer[index].port, buzzer[index].pin, 0);
}
void buzzer_turnOn(uint8_t index) {
    buzzer_resetState(index);
    buzzer[index].is_on = 1;
    HAL_GPIO_WritePin(buzzer[index].port, buzzer[index].pin, 1);
//        HAL_GPIO_WritePin(BUZZER_0_GPIO_Port, BUZZER_0_Pin, 1);
    // TODO: reset state of buzzer (It's unnecessary, but confirm state & counter)
}
void buzzer_turnOff(uint8_t index) {
    buzzer_resetState(index);
    // TODO: reset state of buzzer
}
// Software timer get state to run counter (or not)
int is_buzzer_on(uint8_t index) {
    return (buzzer[index].is_on);
}
// Place this function in MAIN-LOOP
void buzzer_processing(uint8_t index) {
    if(buzzer[index].is_on) {
        // TODO
        switch(buzzer[index].onState) {
            case SLOW:
            if(timer_flag_buzzerState[index]) {
                buzzer[index].onState = ALMOSTSLOW;
                setTimerBuzzerState(index, BUZZER_CHANGE_STATE_PERIOD);
                setTimerBuzzer(index, getNetxRingTime(buzzer[index].onState, 0));
                HAL_GPIO_WritePin(buzzer[index].port, buzzer[index].pin, 1);
            }
            else if(timer_flag_buzzer[index]) {
                if(HAL_GPIO_ReadPin(buzzer[index].port, buzzer[index].pin)) {
                    setTimerBuzzer(index, getNetxRingTime(buzzer[index].onState, 1));
                    HAL_GPIO_WritePin(buzzer[index].port, buzzer[index].pin, 0);
                }
                else {
                    setTimerBuzzer(index, getNetxRingTime(buzzer[index].onState, 0));
                    HAL_GPIO_WritePin(buzzer[index].port, buzzer[index].pin, 1);
                }
            }
            break;
        case ALMOSTSLOW:
            if(timer_flag_buzzerState[index]) {
                buzzer[index].onState = MEDIUM;
                setTimerBuzzerState(index, BUZZER_CHANGE_STATE_PERIOD);
                setTimerBuzzer(index, getNetxRingTime(buzzer[index].onState, 0));
                HAL_GPIO_WritePin(buzzer[index].port, buzzer[index].pin, 1);
            }
            else if(timer_flag_buzzer[index]) {
                if(HAL_GPIO_ReadPin(buzzer[index].port, buzzer[index].pin)) {
                    setTimerBuzzer(index, getNetxRingTime(buzzer[index].onState, 1));
                    HAL_GPIO_WritePin(buzzer[index].port, buzzer[index].pin, 0);
                }
                else {
                    setTimerBuzzer(index, getNetxRingTime(buzzer[index].onState, 0));
                    HAL_GPIO_WritePin(buzzer[index].port, buzzer[index].pin, 1);
                }
            }
            break;
        case MEDIUM:
            if(timer_flag_buzzerState[index]) {
                buzzer[index].onState = ALMOSTFAST;
                setTimerBuzzerState(index, BUZZER_CHANGE_STATE_PERIOD);
                setTimerBuzzer(index, getNetxRingTime(buzzer[index].onState, 0));
                HAL_GPIO_WritePin(buzzer[index].port, buzzer[index].pin, 1);
            }
            else if(timer_flag_buzzer[index]) {
                if(HAL_GPIO_ReadPin(buzzer[index].port, buzzer[index].pin)) {
                    setTimerBuzzer(index, getNetxRingTime(buzzer[index].onState, 1));
                    HAL_GPIO_WritePin(buzzer[index].port, buzzer[index].pin, 0);
                }
                else {
                    setTimerBuzzer(index, getNetxRingTime(buzzer[index].onState, 0));
                    HAL_GPIO_WritePin(buzzer[index].port, buzzer[index].pin, 1);
                }
            }
            break;
        case ALMOSTFAST:
            if(timer_flag_buzzerState[index]) {
                buzzer[index].onState = FAST;
                setTimerBuzzerState(index, BUZZER_CHANGE_STATE_PERIOD);
                setTimerBuzzer(index, getNetxRingTime(buzzer[index].onState, 0));
                HAL_GPIO_WritePin(buzzer[index].port, buzzer[index].pin, 1);
            }
            else if(timer_flag_buzzer[index]) {
                if(HAL_GPIO_ReadPin(buzzer[index].port, buzzer[index].pin)) {
                    setTimerBuzzer(index, getNetxRingTime(buzzer[index].onState, 1));
                    HAL_GPIO_WritePin(buzzer[index].port, buzzer[index].pin, 0);
                }
                else {
                    setTimerBuzzer(index, getNetxRingTime(buzzer[index].onState, 0));
                    HAL_GPIO_WritePin(buzzer[index].port, buzzer[index].pin, 1);
                }
            }
            break;
        case FAST:
            if(timer_flag_buzzerState[index]) {
                buzzer[index].onState = SLOW;
                setTimerBuzzerState(index, BUZZER_CHANGE_STATE_PERIOD);
                setTimerBuzzer(index, getNetxRingTime(buzzer[index].onState, 0));
                HAL_GPIO_WritePin(buzzer[index].port, buzzer[index].pin, 1);
//                buzzer_turnOff(index);
            }
            else if(timer_flag_buzzer[index]) {
                if(HAL_GPIO_ReadPin(buzzer[index].port, buzzer[index].pin)) {
                    setTimerBuzzer(index, getNetxRingTime(buzzer[index].onState, 1));
                    HAL_GPIO_WritePin(buzzer[index].port, buzzer[index].pin, 0);
                }
                else {
                    setTimerBuzzer(index, getNetxRingTime(buzzer[index].onState, 0));
                    HAL_GPIO_WritePin(buzzer[index].port, buzzer[index].pin, 1);

                }
            }
            break;
    }
    }
}

// Below function will place in software timer


void setTimerBuzzer(uint8_t buzzer_index, int time) {
    timer_counter_buzzer[buzzer_index] = time * FREQUENCY_OF_TIM3 / 1000.0;
    timer_flag_buzzer[buzzer_index] = 0;
}
void setTimerBuzzerState(uint8_t buzzer_index, int time) {
    timer_counter_buzzerState[buzzer_index] = time * FREQUENCY_OF_TIM3 / 1000.0;
    timer_flag_buzzerState[buzzer_index] = 0;
}
void timerBuzzerInit() {
    for(int index = 0; index < NUMBER_OF_BUZZER; index++) {
        setTimerBuzzer(index, BUZZER_SLOW_PERIOD * ((BUZZER_SLOW_PERIOD / 100.0)));
        setTimerBuzzerState(index, BUZZER_CHANGE_STATE_PERIOD);
    }
}
void timerBuzzerRun(void) {
    for(int buzzer_index = 0; buzzer_index < NUMBER_OF_BUZZER; buzzer_index++) {
        if(is_buzzer_on(buzzer_index)) {    //
            timer_counter_buzzer[buzzer_index]--;
            if(timer_counter_buzzer[buzzer_index] == 0) {
                timer_flag_buzzer[buzzer_index] = 1;
            }
            timer_counter_buzzerState[buzzer_index]--;
            if(timer_counter_buzzerState[buzzer_index] == 0) {
                timer_flag_buzzerState[buzzer_index] = 1;
            }
        }
    }
}
