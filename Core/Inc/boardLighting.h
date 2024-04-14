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
#include "mcp23017.h"
#include "GameCharacters.h"
#include <string>

//void displayMap(TIM_HandleTypeDef htim1, TIM_HandleTypeDef htim3,TIM_HandleTypeDef htim4, TIM_HandleTypeDef htim5,uint8_t* mapBuffer, size_t bufferSize ){};
void displayMap(TIM_HandleTypeDef htim1, TIM_HandleTypeDef htim3, uint8_t* mapBuffer, size_t bufferSize );


void mapHexesToBuffer(uint8_t* mapBuffer, uint8_t* mapBufferPrev, std::vector<Hexagon*> hexes, int colorMod);

void mapHexesToMap(std::vector<Hexagon*> hexes, GameMap *map, HexagonType type);

void mapToBuffer(GameMap *map, uint8_t* mapBuffer);

void bufferToMap(GameMap *map,uint8_t* mapBuffer);

GameMap* movementMode(TIM_HandleTypeDef htim1, TIM_HandleTypeDef htim3,MCP23017_HandleTypeDef hmcps1[8], MCP23017_HandleTypeDef hmcps2[8], GameMap *map, Hexagon* currHex,Character* _character, int* movement);

void clearMap(TIM_HandleTypeDef htim1, TIM_HandleTypeDef htim3);

GameMap* combatMode(TIM_HandleTypeDef htim1, TIM_HandleTypeDef htim3,MCP23017_HandleTypeDef hmcps1[8], MCP23017_HandleTypeDef hmcps2[8], GameMap *map, Hexagon* currHex, Character* _character);

void blinkLED(uint8_t* mapCharBuffer , int row, int col, int type);

int getRoll(const std::string& inputStr);

void attackHit(uint8_t* mapCharBuffer , int row, int col, int type);

#endif /* INC_WS2812B_H_ */
