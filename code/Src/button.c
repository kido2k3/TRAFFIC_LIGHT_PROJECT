/*
 * button.c
 *
 *  Created on: Oct 30, 2023
 *      Author: HP
 */

#include"button.h"

#define NUMBER_OF_BUTTON 	3

#define RELEASE				1
#define PRESSED				0
#define LONG_PRESSED_TIME	150 // 1.5s

/*
@brief: reg - register array to debounce button
		is_pressed - flag whether button is pressed or release
		is_long_pressed - flag whether button is long-pressed or not
		timer - if pressed over timer, chage is_long_pressed to true
		port - port of button
		pin - pin of button
*/
struct {
	bool reg[3];
	bool is_pressed;
	bool is_long_pressed;
	unsigned int timer;

	GPIO_TypeDef *port;
	uint16_t pin;
} button[NUMBER_OF_BUTTON];
/*
 * button[0]: transitioning-mode button
 * button[1]: increasing-value button
 * button[2]: setting-value button*/
void button_init(void) {
	for (int i = 0; i < NUMBER_OF_BUTTON; i++) {
		button[i].reg[0] = button[i].reg[1] = button[i].reg[2] = RELEASE;
		button[i].is_long_pressed = 0;
		button[i].is_pressed = 0;
		button[i].timer = LONG_PRESSED_TIME;
	}
	// port and pin were matched by hand
	button[0].port = BUTTON1_PORT;
	button[0].pin = BUTTON1;
	button[1].port = BUTTON2_PORT;
	button[1].pin = BUTTON2;
	button[2].port = BUTTON3_PORT;
	button[2].pin = BUTTON3;
}
/*
 * @brief:	read the value of all buttons
 * @para:	none
 * @retval:	none
 * */
void button_read(void) {
	for (unsigned i = 0; i < NUMBER_OF_BUTTON; i++) {
		button[i].reg[0] = button[i].reg[1];
		button[i].reg[1] = button[i].reg[2];
		button[i].reg[2] = HAL_GPIO_ReadPin(button[i].port, button[i].pin);
		if (button[i].reg[0] == button[i].reg[1]
				&& button[i].reg[1] == button[i].reg[2]) {
			//stable state, not bouncing
			if (button[i].reg[2] == PRESSED) {
				button[i].is_pressed = 1;
				//decrease counter to toggle is_long_pressed flag
				if (button[i].timer > 0) {
					button[i].timer--;
				} else {
					button[i].is_long_pressed = 1;
				}
			} else {
				button[i].is_long_pressed = button[i].is_pressed = 0;
				button[i].timer = LONG_PRESSED_TIME;
			}
		}
	}
}
/*
 * @brief:	return the is_pressed flag
 * @para:	i - id of button
 * @retval:	is_pressed (0: released, 1: pressed)
 * */
bool is_button_pressed(unsigned i) {
	if (i >= NUMBER_OF_BUTTON)
		return ERROR;
	return button[i].is_pressed;
}
/*
 * @brief:	return the is_long_pressed flag
 * @para:	i - id of button
 * @retval:	is_pressed (1: long-pressed, 0: not)
 * */
bool is_button_long_pressed(unsigned i) {
	if (i >= NUMBER_OF_BUTTON)
		return ERROR;
	return button[i].is_long_pressed;
}
