/*
 * ws2812b.h
 *
 *  Created on: Mar 19, 2024
 *      Author: lando
 */

#ifndef INC_WS2812B_H_
#define INC_WS2812B_H_



//INCLUDES
#include "stm32f4xx_hal.h"
#include "math.h"
#include "stdio.h"

//defines and initializations
#define MAX_LED 16
#define USE_BRIGHTNESS 1
#define PI 3.14159265


uint8_t LED_Data[MAX_LED][4];
uint8_t LED_Mod[MAX_LED][4]; //for brightness
int datasentflag = 0;
uint16_t pwmData[(24*MAX_LED)+50]; //50 added for reset code


//functions


void Set_LED(int LEDnum, int Red, int Green, int Blue)
{
	LED_Data[LEDnum][0] = LEDnum;
	LED_Data[LEDnum][1] = Green;
	LED_Data[LEDnum][2] = Red;
	LED_Data[LEDnum][3] = Blue;
}


void Set_Brightness(int brightness)
{
#if USE_BRIGHTNESS
	if (brightness > 45) brightness = 45;
	for (int i=0; i<MAX_LED; i++)
	{
		LED_Mod[i][0] = LED_Data[i][0];
		for (int j=1; j< 4; j++)
		{
			float angle = 90-brightness; //in degrees
			angle = angle*PI / 180;  //in rad
			LED_Mod[i][j] = (LED_Data[i][j])/(tan(angle));

		}
	}

#endif
}

void WS2812_Send (TIM_HandleTypeDef* htim1, int channelNum)
{
	uint32_t indx=0;
	uint32_t color;
	unsigned int channels[] = {TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4};

	for (int i= 0; i<MAX_LED; i++)
	{
#if USE_BRIGHTNESS
		color = ((LED_Mod[i][1]<<16) | (LED_Mod[i][2]<<8) | (LED_Mod[i][3]));
#else
		color = ((LED_Data[i][1]<<16) | (LED_Data[i][2]<<8) | (LED_Data[i][3]));
#endif

		for (int i=23; i>=0; i--)
		{
			if (color&(1<<i))
			{
				pwmData[indx] = 40; //14;// 2/3 of 20 = 16MHz / 800kHz  // 2/3 of 105 = 84MHz / 800kHz
			}

			else pwmData[indx] = 20; //7; // 1/3 of 20 = 16MHz / 800kHz // 1/3 of 105 = 84MHz / 800kHz

			indx++;
		}

	}

	//reset code see datasheet
	for (int i=0; i<50; i++)
	{
		pwmData[indx] = 0;
		indx++;
	}
	//channels[channelNum-1]
	HAL_TIM_PWM_Start_DMA(htim1, channels[channelNum-1], (uint32_t *)pwmData, indx);
	while (!datasentflag){};
	datasentflag = 0;
}








#endif /* INC_WS2812B_H_ */
