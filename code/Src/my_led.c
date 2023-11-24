/*
 * segment_led.c
 *
 *  Created on: Sep 15, 2023
 *      Author: HP
 */

#include <my_led.h>

/*
 * bief: traffic light struct
 * */
struct {
	GPIO_TypeDef *green_port;
	GPIO_TypeDef *red_port;
	uint16_t green;
	uint16_t red;
} traffic_light[2], pedestrian_light;

void init_traffic_light(void);

/*
 * @brief: 	display traffic light function
 * @para:	i - id of traffic light
 * 			red, green - state of red and green led (1: on, 0: off)
 * @retval:	none*/
void control_traffic_light(unsigned i, GPIO_PinState red, GPIO_PinState green) {
	HAL_GPIO_WritePin(traffic_light[i].red_port, traffic_light[i].red, !red);
	HAL_GPIO_WritePin(traffic_light[i].green_port, traffic_light[i].green, !green);
}
/*
 * @brief: 	display pedestrian light function
 * @para:	red, green - state of red and green led (1: on, 0: off)
 * @retval:	none*/
void control_pedestrian_light(GPIO_PinState red, GPIO_PinState green) {
	HAL_GPIO_WritePin(pedestrian_light.red_port, pedestrian_light.red, !red);
	HAL_GPIO_WritePin(pedestrian_light.green_port, pedestrian_light.green, !green);
}
void init_led(void) {
	init_traffic_light();
}
void init_traffic_light(void) {
	traffic_light[0].green_port = TL_GREEN_PORT1;
	traffic_light[0].green = TL_GREEN1;
	traffic_light[0].red_port = TL_RED_PORT1;
	traffic_light[0].red = TL_RED1;
	traffic_light[1].green_port = TL_PORT2;
	traffic_light[1].red_port = TL_PORT2;
	traffic_light[1].green = TL_GREEN2;
	traffic_light[1].red = TL_RED2;

	pedestrian_light.green_port = TL_PED_GREEN_PORT;
	pedestrian_light.red_port = TL_PED_RED_PORT;
	pedestrian_light.red = TL_PED_RED;
	pedestrian_light.green = TL_PED_GREEN;
}



