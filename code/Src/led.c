/*
 * segment_led.c
 *
 *  Created on: Sep 15, 2023
 *      Author: HP
 */

#include <led.h>

#define NUMBER_OF_7_LED 4

/*
 * bief: 7-segment led struct
 * */
struct {
	GPIO_TypeDef *port;
	uint16_t pin_a;
	uint16_t pin_b;
	uint16_t pin_c;
	uint16_t pin_d;
	uint16_t pin_e;
	uint16_t pin_f;
	uint16_t pin_g;
} led;
/*
 * bief: traffic light struct
 * */
struct{
	GPIO_TypeDef *port;
	uint16_t green;
	uint16_t yellow;
	uint16_t red;
} traffic_light[2];

uint8_t led_buffer[NUMBER_OF_7_LED] = { 7,4,8,0 };
unsigned idx_7led = 0;


void display7SEG(unsigned short i);
void update7led(uint8_t i);
void init_traffic_light(void);
void init7SEG(void);


/*
 * @brief: 	display traffic light function
 * @para:	i - id of traffic light
 * 			red, yellow, green - state of red, yellow and green led (1: on, 0: off)
 * @retval:	none*/
void control_traffic_light(unsigned i, GPIO_PinState red, GPIO_PinState yellow, GPIO_PinState green){
	HAL_GPIO_WritePin(traffic_light[i].port, traffic_light[i].red, !red);
	HAL_GPIO_WritePin(traffic_light[i].port, traffic_light[i].green, !green);
	HAL_GPIO_WritePin(traffic_light[i].port, traffic_light[i].yellow, !yellow);
}
void init_led(void){
	init_traffic_light();
	init7SEG();
}

void init_traffic_light(void){
	traffic_light[0].port = TL_PORT1;
	traffic_light[0].green = TL_GREEN1;
	traffic_light[0].yellow =TL_YELLOW1;
	traffic_light[0].red =TL_RED1;
	traffic_light[1].port = TL_PORT2;
	traffic_light[1].green=TL_GREEN2;
	traffic_light[1].yellow=TL_YELLOW2;
	traffic_light[1].red=TL_RED2;
}
/*
 * @brief: 	call buffer function and increase id of 7-seg leds
 * @para:	none
 * @retval:	none*/
void scan7SEG(void){
	update7led(idx_7led++);
	if(idx_7led >= 4){
		idx_7led = 0;
	}
}
/*
 * @brief: 	update buffer of 4 7-segment leds
 * @para:	value1 < 100 for the 1st and 2nd led
 * 			value2 < 100 for the 3rd and 4th led
 * @retval:	none*/
void update_led_buf(unsigned value1, unsigned value2) {
	if (0 < value1 && value1 < 100 && 0 < value2 && value2 < 100) {
		led_buffer[0] = value1 / 10;
		led_buffer[1] = value1 % 10;
		led_buffer[2] = value2 / 10;
		led_buffer[3] = value2 % 10;
	}
}
void off_all7led(void) {
	uint16_t pin = SEG7_1 | SEG7_2 | SEG7_3 | SEG7_4;
	HAL_GPIO_WritePin(led.port, pin, 1);
}
void update7led(uint8_t i) {
	switch (i) {
	case 0:
		off_all7led();
		HAL_GPIO_WritePin(led.port, SEG7_1, 0);
		display7SEG(led_buffer[0]);
		break;
	case 1:
		off_all7led();
		HAL_GPIO_WritePin(led.port, SEG7_2, 0);
		display7SEG(led_buffer[1]);
		break;
	case 2:
		off_all7led();
		HAL_GPIO_WritePin(led.port, SEG7_3, 0);
		display7SEG(led_buffer[2]);
		break;
	case 3:
		off_all7led();
		HAL_GPIO_WritePin(led.port, SEG7_4, 0);
		display7SEG(led_buffer[3]);
		break;
	default:
		off_all7led();
	}
}
void display(GPIO_PinState a, GPIO_PinState b, GPIO_PinState c, GPIO_PinState d,
		GPIO_PinState e, GPIO_PinState f, GPIO_PinState g) {
	HAL_GPIO_WritePin(led.port, led.pin_a, a);
	HAL_GPIO_WritePin(led.port, led.pin_b, b);
	HAL_GPIO_WritePin(led.port, led.pin_c, c);
	HAL_GPIO_WritePin(led.port, led.pin_d, d);
	HAL_GPIO_WritePin(led.port, led.pin_e, e);
	HAL_GPIO_WritePin(led.port, led.pin_f, f);
	HAL_GPIO_WritePin(led.port, led.pin_g, g);
}
void display7SEG(unsigned short i) {
	switch (i) {
	case 0:
		display(0, 0, 0, 0, 0, 0, 1);
		break;
	case 1:
		display(1, 0, 0, 1, 1, 1, 1);
		break;
	case 2:
		display(0, 0, 1, 0, 0, 1, 0);
		break;
	case 3:
		display(0, 0, 0, 0, 1, 1, 0);
		break;
	case 4:
		display(1, 0, 0, 1, 1, 0, 0);
		break;
	case 5:
		display(0, 1, 0, 0, 1, 0, 0);
		break;
	case 6:
		display(0, 1, 0, 0, 0, 0, 0);
		break;
	case 7:
		display(0, 0, 0, 1, 1, 1, 1);
		break;
	case 8:
		display(0, 0, 0, 0, 0, 0, 0);
		break;
	case 9:
		display(0, 0, 0, 0, 1, 0, 0);
		break;
	default:
		display(1, 1, 1, 1, 1, 1, 1);
	}
}
void init7SEG(void) {
	led.port = SEG7_PORT;
	led.pin_a = SEG7_A;
	led.pin_b = SEG7_B;
	led.pin_c = SEG7_C;
	led.pin_d = SEG7_D;
	led.pin_e = SEG7_E;
	led.pin_f = SEG7_F;
	led.pin_g = SEG7_G;
}
