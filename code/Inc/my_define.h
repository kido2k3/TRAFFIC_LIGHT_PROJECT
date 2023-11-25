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
#define BUTTON1_PORT GPIOA
#define BUTTON1 GPIO_PIN_1
#define BUTTON2_PORT GPIOA
#define BUTTON2 GPIO_PIN_4
#define BUTTON3_PORT GPIOB
#define BUTTON3 GPIO_PIN_0
// define pedestrian button - pull up
#define BUTTON4_PORT GPIOA
#define BUTTON4 GPIO_PIN_0
// define output

#define TL_GREEN_PORT1 GPIOA // Traffic light 1st
#define TL_GREEN1 GPIO_PIN_10
#define TL_RED_PORT1 GPIOB // Traffic light 1st
#define TL_RED1 GPIO_PIN_3

#define TL_PORT2 GPIOB // Traffic light 2nd
#define TL_GREEN2 GPIO_PIN_4
#define TL_RED2 GPIO_PIN_5

#define TL_PED_GREEN_PORT GPIOB // Pedestrian light
#define TL_PED_GREEN GPIO_PIN_10
#define TL_PED_RED_PORT GPIOA // Pedestrian light
#define TL_PED_RED GPIO_PIN_8
#define ERROR 0xff

typedef uint8_t bool;
#endif /* INC_MY_DEFINE_H_ */
