/*
 * segment_led.h
 *
 *  Created on: Sep 15, 2023
 *      Author: HP
 */

#ifndef SEGMENT_LED_H_
#define SEGMENT_LED_H_

#include <stdint.h>
#include "port.h"


void scan7SEG(void);
void init_led(void);
void control_traffic_light(unsigned i, GPIO_PinState red, GPIO_PinState yellow, GPIO_PinState green);
void update_led_buf(unsigned value1, unsigned value2);
void off_all7led(void);
#endif /* SEGMENT_LED_H_ */
