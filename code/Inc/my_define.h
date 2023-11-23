/*
 * my_define.h
 *
 *  Created on: Nov 18, 2023
 *      Author: HP
 */

#ifndef INC_MY_DEFINE_H_
#define INC_MY_DEFINE_H_

#include "main.h"

// define input
// All buttons are pull-up
#define BUTTON1_PORT		GPIOA
#define BUTTON1				GPIO_PIN_7
#define BUTTON2_PORT		GPIOA
#define BUTTON2				GPIO_PIN_8
#define BUTTON3_PORT		GPIOA
#define BUTTON3				GPIO_PIN_9
// define pedestrian button - pull up
#define BUTTON4_PORT		GPIOA
#define BUTTON4				GPIO_PIN_10
// define output

#define SEG7_PORT			GPIOB
#define SEG7_A				GPIO_PIN_0
#define SEG7_B				GPIO_PIN_1
#define SEG7_C				GPIO_PIN_2
#define SEG7_D				GPIO_PIN_3
#define SEG7_E				GPIO_PIN_4
#define SEG7_F				GPIO_PIN_5
#define SEG7_G				GPIO_PIN_6
#define SEG7_1				GPIO_PIN_7
#define SEG7_2				GPIO_PIN_8
#define SEG7_3				GPIO_PIN_9
#define SEG7_4				GPIO_PIN_10

#define TL_PORT1			GPIOA	//Traffic light 1st
#define TL_GREEN1			GPIO_PIN_1
#define TL_RED1				GPIO_PIN_3
#define TL_PORT2			GPIOA	//Traffic light 2nd
#define TL_GREEN2			GPIO_PIN_4
#define TL_RED2				GPIO_PIN_11
#define TL_PEDESTRIAN_PORT	GPIOA// Pedestrian light
#define TL_PEDESTRIAN_GREEN	GPIO_PIN_2
#define TL_PEDESTRIAN_RED	GPIO_PIN_5
#define ERROR				0xff

typedef uint8_t bool;

typedef uint8_t bool;
#endif /* INC_MY_DEFINE_H_ */
