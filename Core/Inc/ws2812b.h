/*
 * ws2812b.h
 *
 *  Created on: Mar 19, 2024
 *      Author: lando
 */

#ifndef INC_WS2812B_HH
#define INC_WS2812B_HH



//INCLUDES
#include "stm32f4xx_hal.h"
#include "math.h"
#include "stdio.h"

//defines and initializations
#define MAX_LED 32
#define USE_BRIGHTNESS 0
#define PI 3.14159265

extern uint8_t LED_Data[MAX_LED][4];
extern uint8_t LED_Mod[MAX_LED][4]; //for brightness
extern int datasentflag;
extern uint16_t pwmData[(24*MAX_LED)]; //50 added for reset code

//functions
void Set_LED(int LEDnum, int Red, int Green, int Blue);


void Set_Brightness(int brightness);

void WS2812_Send (TIM_HandleTypeDef* htim1, int channelNum);




#endif /* INC_WS2812B_H_ */
