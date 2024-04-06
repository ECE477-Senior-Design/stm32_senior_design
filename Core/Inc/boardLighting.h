/*
 * boardLighting.h
 *
 *  Created on: Apr 3, 2024
 *      Author: lando
 */

#ifndef BOARDLIGHTING_H
#define BOARDLIGHTING_H
#include <cstddef>
#include "stm32f4xx_hal.h"
#include "GameMap.h"
#include <vector>
#include <cstring>



//void displayMap(TIM_HandleTypeDef htim1, TIM_HandleTypeDef htim3,TIM_HandleTypeDef htim4, TIM_HandleTypeDef htim5,uint8_t* mapBuffer, size_t bufferSize ){};
void displayMap(TIM_HandleTypeDef htim1, TIM_HandleTypeDef htim3, uint8_t* mapBuffer, size_t bufferSize );


void mapHexes(uint8_t* mapBuffer, uint8_t* mapBufferPrev, std::vector<Hexagon*> hexes, int colorMod);

#endif /* INC_WS2812B_H_ */
