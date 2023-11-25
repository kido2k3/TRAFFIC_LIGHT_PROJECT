/*
 * fsm.c
 *
 *  Created on: Oct 30, 2023
 *      Author: HP
 */

#include <my_fsm.h>

enum {
	release, pressed, long_pressed
} /*state variable of button*/button_st[4];

enum {
	TRAFFIC_LIGHT,
	RED_ADJUSTMENT,
	YELLOW_ADJUSTMENT,
	GREEN_ADJUSTMENT,
	SET_VALUE,
	INCREASE_BY_1,
	INCREASE_BY_1_OVER_TIME,
	MANUALLY_SET
} /*state variable of system*/light_st = TRAFFIC_LIGHT,
/* previous state variable of system*/light_pre_st = TRAFFIC_LIGHT;
enum {
	RED_GREEN, RED_YELLOW, GREEN_RED, YELLOW_RED, UNEQUAL
} /* state variable of traffic light*/tl_st = RED_GREEN, man_tl_st = RED_GREEN;
enum {
	ON, OFF
} /* state variable of single led*/led_st, pedestrian_st = OFF;
#define RED_TIME_INIT 			5
#define GREEN_TIME_INIT 		4
#define YELLOW_TIME_INIT 		1
#define PEDESTRIAN_TIMER		30

unsigned red_time = RED_TIME_INIT;
unsigned green_time = GREEN_TIME_INIT;
unsigned yellow_time = YELLOW_TIME_INIT;

unsigned red_time_buffer = RED_TIME_INIT;
unsigned green_time_buffer = GREEN_TIME_INIT;
unsigned yellow_time_buffer = YELLOW_TIME_INIT;

unsigned traffic_light_timer1 = RED_TIME_INIT;
unsigned traffic_light_timer2 = GREEN_TIME_INIT;

unsigned pedestrian_timer = 0; // timer to auto turn off pedestrian light when no one press for a while

bool flag_toggle_led = 1;
bool flag_countdown = 1;
bool flag_increase_over_time = 1;
bool flag_pedestrian_on = 1;

bool button0_fsm(void);
bool button1_fsm(void);
bool button2_fsm(void);
bool button3_fsm(void); // pedestrian button fsm
bool manually_change_fsm(void);
bool manually_set_fsm(void);
void increase_value(void);
void task_toggle_led(void);
void task_countdown_1sec(void);
void task_increase_over_time(void);
void task_countdown_pedestrian_timer(void);
void fsm_led(void);
void traffic_light_fsm(void);
void manually_traffic_state(void);
/**
 * @brief  Top-layer finite state machine
 * @param  None
 * @retval None
 */
void fsm(void) {
	switch (light_st) {
	case TRAFFIC_LIGHT:
		if (red_time != green_time + yellow_time) {
			// off all leds
			control_traffic_light(0, 0, 0);
			control_traffic_light(1, 0, 0);
		} else {
			// decrease timer every 1s
			if (flag_countdown == 1) {
				flag_countdown = 0;
				sch_add_task(task_countdown_1sec, ONE_SECOND, 0);
			}
			traffic_light_fsm();
		}
		if(!pedestrian_timer){
			control_pedestrian_light(0, 0);
		} else if(pedestrian_timer){
			if (flag_pedestrian_on) {
				flag_pedestrian_on = 0;
				sch_add_task(task_countdown_pedestrian_timer, ONE_SECOND, 0);
			}
			switch(tl_st){
			case RED_GREEN:
				control_pedestrian_light(0, 1);
				break;
			case RED_YELLOW:
				control_pedestrian_light(0, 1);
				break;
			case GREEN_RED:
				control_pedestrian_light(1, 0);
				break;
			case YELLOW_RED:
				control_pedestrian_light(1, 0);
				break;
			default:
				break;
			}
		}
		// transition to adjustment mode
		button0_fsm();
		// transistion to manual setting mode
		button2_fsm();
		// button to control pedestrian led
		button3_fsm();
		break;
	case RED_ADJUSTMENT:
		// update buffer of red with the condition that previous state has to be different from changing-value states
		if (light_pre_st != INCREASE_BY_1 && light_pre_st != SET_VALUE
				&& light_pre_st != INCREASE_BY_1_OVER_TIME) {
			red_time_buffer = red_time;
		}
		// update buffer of four 7-seg leds: value of red buffer and the mode (2)

		fsm_led();
		// transition mode function
		button0_fsm();
		button1_fsm();
		button2_fsm();

		break;
	case YELLOW_ADJUSTMENT:
		// update buffer of yellow with the condition that previous state has to be different from changing-value states
		if (light_pre_st != INCREASE_BY_1 && light_pre_st != SET_VALUE
				&& light_pre_st != INCREASE_BY_1_OVER_TIME)
			yellow_time_buffer = yellow_time;
		// update buffer of four 7-seg leds: value of yellow buffer and the mode (3)

		fsm_led();
		// transition mode function
		button0_fsm();
		button1_fsm();
		button2_fsm();

		break;
	case GREEN_ADJUSTMENT:
		// update buffer of green with the condition that previous state has to be different from changing-value states
		if (light_pre_st != INCREASE_BY_1 && light_pre_st != SET_VALUE
				&& light_pre_st != INCREASE_BY_1_OVER_TIME)
			green_time_buffer = green_time;
		// update buffer of four 7-seg leds: value of yellow buffer and the mode (4)

		fsm_led();
		// transition mode function
		button0_fsm();
		button1_fsm();
		button2_fsm();

		break;
	case SET_VALUE:
		// update the time value based-on previous state
		if (light_pre_st == RED_ADJUSTMENT) {
			red_time = red_time_buffer;
		} else if (light_pre_st == YELLOW_ADJUSTMENT) {
			yellow_time = yellow_time_buffer;
		} else if (light_pre_st == GREEN_ADJUSTMENT) {
			green_time = green_time_buffer;
		}

		light_st = light_pre_st;
		light_pre_st = SET_VALUE;
		break;
	case INCREASE_BY_1:
		// increase the time value based-on previous state (short-pressed)
		increase_value();
		light_st = light_pre_st;
		light_pre_st = INCREASE_BY_1;
		break;
	case INCREASE_BY_1_OVER_TIME:
		// increase the time value every 0.25s based-on previous state (short-pressed)
		if (light_pre_st == RED_ADJUSTMENT) {

		} else if (light_pre_st == YELLOW_ADJUSTMENT) {

		} else if (light_pre_st == GREEN_ADJUSTMENT) {

		}
		if (flag_increase_over_time == 1) {
			flag_increase_over_time = 0;
			sch_add_task(task_increase_over_time, INCREASE_TIME, 0);
		}
		button1_fsm();
		break;
	case MANUALLY_SET:
		manually_traffic_state();
		if(!pedestrian_timer){
			control_pedestrian_light(0, 0);
		} else if(pedestrian_timer){
			if (flag_pedestrian_on) {
				flag_pedestrian_on = 0;
				sch_add_task(task_countdown_pedestrian_timer, ONE_SECOND, 0);
			}
			switch(man_tl_st){
			case RED_GREEN:
				control_pedestrian_light(0, 1);
				break;
			case RED_YELLOW:
				control_pedestrian_light(0, 1);
				break;
			case GREEN_RED:
				control_pedestrian_light(1, 0);
				break;
			case YELLOW_RED:
				control_pedestrian_light(1, 0);
				break;
			default:
				break;
			}
		}
		//control traffic light
		button1_fsm();
		//return to traffic light mode
		button2_fsm();
		// button to control pedestrian led
		button3_fsm();
		break;

	}
}

/*
 * @brief: 	finite state machine to control behavior of traffic light
 * @para:	none
 * @retval:	none*/
void traffic_light_fsm(void) {
	switch (tl_st) {
	case RED_GREEN:
		control_traffic_light(0, 1, 0);
		control_traffic_light(1, 0, 1);
		if (traffic_light_timer2 <= 0) {
			traffic_light_timer2 = yellow_time;
			tl_st = RED_YELLOW;
		}
		break;
	case RED_YELLOW:
		control_traffic_light(0, 1, 0);
		control_traffic_light(1, 1, 1);
		if (traffic_light_timer2 <= 0) {
			traffic_light_timer1 = green_time;
			traffic_light_timer2 = red_time;
			tl_st = GREEN_RED;
		}
		break;
	case GREEN_RED:
		control_traffic_light(0, 0, 1);
		control_traffic_light(1, 1, 0);
		if (traffic_light_timer1 <= 0) {
			traffic_light_timer1 = yellow_time;
			tl_st = YELLOW_RED;
		}
		break;
	case YELLOW_RED:
		control_traffic_light(0, 1, 1);
		control_traffic_light(1, 1, 0);
		if (traffic_light_timer1 <= 0) {
			traffic_light_timer1 = red_time;
			traffic_light_timer2 = green_time;
			tl_st = RED_GREEN;
		}
		break;
	default:
		break;
	}
}
/*
 * @brief: 	finite state machine to display traffic light manually
 * @para:	none
 * @retval:	none*/
void manually_traffic_state(void) {
	switch (man_tl_st) {
	case RED_GREEN:
		control_traffic_light(0, 1, 0);
		control_traffic_light(1, 0, 1);
		break;

	case GREEN_RED:
		control_traffic_light(0, 0, 1);
		control_traffic_light(1, 1, 0);
		break;
	default:
		control_traffic_light(0, 1, 1);
		control_traffic_light(1, 1, 1);
		break;
	}
}
/*
 * @brief: 	finite state machine to switch traffic state manually
 * @para:	none
 * @retval:	none*/
void switch_traffic_state(void) {
	switch (man_tl_st) {
	case RED_GREEN:
		man_tl_st = GREEN_RED;
		break;
	case GREEN_RED:
		man_tl_st = RED_GREEN;
		break;
	default:
		man_tl_st = GREEN_RED;
	}
}
/*@brief:	state machine to blink led in 2Hz
 * @para:	none
 * @retval:	none*/
void fsm_led(void) {
	// transition state in 0.25s
	if (flag_toggle_led) {
		flag_toggle_led = 0;
		sch_add_task(task_toggle_led, TOGGLE_TIME, 0);
	}
	switch (led_st) {
	case ON:
		switch (light_st) {
		case RED_ADJUSTMENT:
			// turn red led on
			control_traffic_light(0, 1, 0);
			control_traffic_light(1, 1, 0);
			break;
		case YELLOW_ADJUSTMENT:
			// turn yellow led on
			control_traffic_light(0, 1, 1);
			control_traffic_light(1, 1, 1);
			break;
		case GREEN_ADJUSTMENT:
			// turn green led on
			control_traffic_light(0, 0, 1);
			control_traffic_light(1, 0, 1);
			break;
		default:
			break;
		}
		break;
	case OFF:
		control_traffic_light(0, 0, 0);
		control_traffic_light(1, 0, 0);
		break;
	}
}
/**
 * @brief  increase the time value based-on previous state (short-pressed)
 * @param  None
 * @retval None
 */
void increase_value(void) {
	if (light_pre_st == RED_ADJUSTMENT) {
		red_time_buffer++;
		red_time_buffer %= 100;
	} else if (light_pre_st == YELLOW_ADJUSTMENT) {
		yellow_time_buffer++;
		yellow_time_buffer %= 100;
	} else if (light_pre_st == GREEN_ADJUSTMENT) {
		green_time_buffer++;
		green_time_buffer %= 100;
	}
}
/*
 * @brief:	mode button fsm - 2 states
 * @para:	none
 * @retval:	1 - successful
 * 			0 - fail
 * */
bool button0_fsm(void) {
	switch (button_st[0]) {
	case release:
		if (is_button_pressed(0) == 1) {
			// to do
			light_pre_st = light_st;
			control_pedestrian_light(0, 0);
			pedestrian_timer = 0;
			switch (light_st) {
			case TRAFFIC_LIGHT:
				light_st = RED_ADJUSTMENT;
				break;
			case RED_ADJUSTMENT:
				light_st = YELLOW_ADJUSTMENT;
				break;
			case YELLOW_ADJUSTMENT:
				light_st = GREEN_ADJUSTMENT;
				break;
			case GREEN_ADJUSTMENT:
				light_st = TRAFFIC_LIGHT;
				tl_st = RED_GREEN;
				traffic_light_timer1 = red_time;
				traffic_light_timer2 = green_time;
				break;
			default:
				break;
			}
			button_st[0] = pressed;
		} else if (is_button_pressed(0) == ERROR)
			return 0;
		break;
	case pressed:
		if (!is_button_pressed(0)) {
			button_st[0] = release;
		} else {
			return 0;
		}
		break;
	default:
		return 0;
	}
	return 1;
}
/*
 * @brief:	setting-value button fsm - 2 states
 * @para:	none
 * @retval:	1 - successful
 * 			0 - fail
 * */
bool button2_fsm(void) {
	switch (button_st[2]) {
	case release:
		if (is_button_pressed(2) == 1) {
			// to do
			light_pre_st = light_st;
			switch (light_st) {
			case TRAFFIC_LIGHT:
				light_st = MANUALLY_SET;
				break;
			case MANUALLY_SET:
				light_st = TRAFFIC_LIGHT;
				break;
			default:
				light_st = SET_VALUE;

			}
			button_st[2] = pressed;
		} else if (is_button_pressed(2) == ERROR)
			return 0;
		break;
	case pressed:
		if (!is_button_pressed(2)) {
			button_st[2] = release;
		} else {
			return 0;
		}
		break;
	default:
		return 0;
		break;
	}
	return 1;
}
/*
 * @brief:	increasing-value button fsm - 3 states
 * @para:	none
 * @retval:	1 - successful
 * 			0 - fail
 * */
bool button1_fsm(void) {
	switch (button_st[1]) {
	case release:
		if (is_button_pressed(1) == 1) {
			// to do
			light_pre_st = light_st;
			switch (light_st) {
			case MANUALLY_SET:
				switch (man_tl_st) {
				case RED_GREEN:
					man_tl_st = GREEN_RED;
					break;
				case GREEN_RED:
					man_tl_st = RED_GREEN;
					break;
				default:
					man_tl_st = GREEN_RED;
				}
				break;
			default:
				light_st = INCREASE_BY_1;
				break;
			}
			button_st[1] = pressed;
		} else if (is_button_pressed(1) == ERROR)
			return 0;
		break;
	case pressed:
		if (!is_button_pressed(1)) {
			button_st[1] = release;
		} else if (is_button_long_pressed(1) == 1) {
			button_st[1] = long_pressed;
		} else {
			return 0;
		}
		break;
	case long_pressed:
		// to do
		if (light_st != INCREASE_BY_1_OVER_TIME) {
			light_pre_st = light_st;
			light_st = INCREASE_BY_1_OVER_TIME;
		}
		if (!is_button_pressed(1)) {
			light_st = light_pre_st;
			light_pre_st = INCREASE_BY_1_OVER_TIME;
			button_st[1] = release;
		}
		break;
	default:
		return 0;
		break;
	}
	return 1;
}
/*
 * @brief:	pedestrian button
 * @para:	none
 * @retval:	1 - successful
 * 			0 - fail
 * */
bool button3_fsm(void) {
	switch (button_st[3]) {
	case release:
		if (is_button_pressed(3) == 1) {
			// to do
			pedestrian_timer = PEDESTRIAN_TIMER;
//			flag_pedestrian_on = 1;
			button_st[3] = pressed;
		} else if (is_button_pressed(3) == ERROR)
			return 0;
		break;
	case pressed:
		if (!is_button_pressed(2)) {
			button_st[3] = release;
		} else {
			return 0;
		}
		break;
	default:
		return 0;
		break;
	}
	return 1;
}
/*
 * @brief:	manually mode button
 * @para:	none
 * @retval:	1 - successful
 * 			0 - fail
 * */
bool manually_change_fsm(void) {
	switch (button_st[2]) {
	case release:
		if (is_button_pressed(2) == 1) {
			light_pre_st = light_st;
			switch (light_st) {
			case TRAFFIC_LIGHT:
				man_tl_st = RED_GREEN;
				light_st = MANUALLY_SET;
				break;
			case MANUALLY_SET:
				tl_st = RED_GREEN;
				light_st = TRAFFIC_LIGHT;
				break;
			default:
				break;
			}
			button_st[2] = pressed;
		} else if (is_button_pressed(2) == ERROR)
			return 0;
		break;
	case pressed:
		if (!is_button_pressed(2)) {
			button_st[2] = release;
		} else {
			return 0;
		}
		break;
	default:
		return 0;
	}
	return 1;
}

/*
 * @brief:	switch state of led, use to add task
 * @para:	none
 * @retval:	1 - successful
 * 			0 - fail
 * */
void task_toggle_led(void) {
	switch (led_st) {
	case ON:
		led_st = OFF;
		break;
	case OFF:
		led_st = ON;
		break;
	default:
		break;
	}
	flag_toggle_led = 1;
}

/*
 * @brief:	countdown for 1 second
 * @para:	none
 * @retval:	1 - successful
 * 			0 - fail
 * */
void task_countdown_1sec(void) {
	if (traffic_light_timer1 > 0)
		traffic_light_timer1--;
	if (traffic_light_timer2 > 0)
		traffic_light_timer2--;
	flag_countdown = 1;
}
/*
 * @brief:	increase by 1 overtime
 * @para:	none
 * @retval:	1 - successful
 * 			0 - fail
 * */
void task_increase_over_time(void) {
	increase_value();
	flag_increase_over_time = 1;
}
/*
 * @brief:	countdown pedestrian counter each 1 second
 * @para:	none
 * @retval:	1 - successful
 * 			0 - fail
 * */
void task_countdown_pedestrian_timer(void) {
	if (pedestrian_timer) {
		pedestrian_timer--;
	}
	flag_pedestrian_on = 1;
}
