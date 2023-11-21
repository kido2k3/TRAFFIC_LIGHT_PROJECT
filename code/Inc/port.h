/*
 * port.h
 *
 *  Created on: Oct 24, 2023
 *      Author: HP
 */

#ifndef INC_PORT_H_
#define INC_PORT_H_

#include "stm32f1xx_hal.h"

// define input
#define BUTTON1_PORT	GPIOA
#define BUTTON1			GPIO_PIN_7
#define BUTTON2_PORT	GPIOA
#define BUTTON2			GPIO_PIN_8
#define BUTTON3_PORT	GPIOA
#define BUTTON3			GPIO_PIN_9
// define output
#define LED1_PORT		GPIOA
#define LED1			GPIO_PIN_10

#define SEG7_PORT		GPIOB
#define SEG7_A			GPIO_PIN_0
#define SEG7_B			GPIO_PIN_1
#define SEG7_C			GPIO_PIN_2
#define SEG7_D			GPIO_PIN_3
#define SEG7_E			GPIO_PIN_4
#define SEG7_F			GPIO_PIN_5
#define SEG7_G			GPIO_PIN_6
#define SEG7_1			GPIO_PIN_7
#define SEG7_2			GPIO_PIN_8
#define SEG7_3			GPIO_PIN_9
#define SEG7_4			GPIO_PIN_10

#define TL_PORT1		GPIOA	//Traffic light 1st
#define TL_GREEN1		GPIO_PIN_1
#define TL_YELLOW1		GPIO_PIN_2
#define TL_RED1			GPIO_PIN_3
#define TL_PORT2		GPIOA	//Traffic light 2nd
#define TL_GREEN2		GPIO_PIN_4
#define TL_YELLOW2		GPIO_PIN_5
#define TL_RED2			GPIO_PIN_11
#define LED1_PORT		GPIOA
#define LED1			GPIO_PIN_10
#define ERROR			0xff

typedef uint8_t bool;
#endif /* INC_PORT_H_ */
