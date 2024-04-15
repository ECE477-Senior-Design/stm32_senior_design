//MOST RECENT AUTHOR: Landon
//DATE: 4/3/2024


#include <ws2812b.h>
#include "boardLighting.h"
#include <cstddef>
#include "stm32f4xx_hal.h"
#include "main.h"
#include "mcp23017.h"
#include "keypad.h"
#include "lcd.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
extern GameCharacters * characters;



void displayMap(TIM_HandleTypeDef htim1, TIM_HandleTypeDef htim3,uint8_t* mapBuffer, size_t bufferSize ){
	//DISPLAY MAP
	clearMap(htim1,htim3);
	TIM_HandleTypeDef timers[] = {htim1, htim3}; //, htim3, htim4, htim5};4,1,3,5
	int test[256] = {};
	uint32_t color;

	int channels[] = {4,3,2,1, 4, 3, 2, 1}; //only 4 because groups of two so only run 4 times while operating with half of board
	for(int pcb = 0; pcb < ((int)bufferSize / MAX_LED); pcb++){
		for(int led = 0; led < MAX_LED; led++){
			color = ((LED_Data[led][1]<<16) | (LED_Data[led][2]<<8) | (LED_Data[led][3]));
		  switch(mapBuffer[(MAX_LED*pcb) + led]){
			  case BaseHex:
				  Set_LED(led,0,0,0); //nothing/floor
				  test[led + (pcb*32)] = 0;
				  break;

			  case WallHex:
				  Set_LED(led,255,0,255); //wall
				  test[led + (pcb*32)] = 1;
				  break;

			  case PlayerHex:
				 Set_LED(led,0, 0, 255); //character/player
				 test[led + (pcb*32)] = 2;
				  break;

			 case MonsterHex:
				 Set_LED(led,255,0,0); //enemy
				 test[led + (pcb*32)] = 3;
				  break;

			 case ChestHex:
				 Set_LED(led,100,100,0); //chest
				 test[led + (pcb*32)] = 4;
				  break;

			 case MoveHex:
				 Set_LED(led,0, 255, 0); //possible moves
				 test[led + (pcb*32)] = 5;
				 break;

//			 case PlayerHexTurn:
//				 Set_LED(led,0, 255, 0); //hex of player when its their turn
//				 test[led + (pcb*32)] = 5;
//				 break;

			 default:
				 Set_LED(led,0,0,0); //default off
				 test[led + (pcb*32)] = 0;
				  break;

		  }
		}
		int timer = pcb / 4;
		Set_Brightness(20);
		WS2812_Send(&(timers[timer]),channels[pcb]);
	}
}



void mapHexesToBuffer(uint8_t* mapBuffer, std::vector<Hexagon*> hexes, int colorMod){
	//std::memcpy(mapBufferPrev, mapBuffer, sizeof(uint8_t) * 256);
	int sizeHexes = hexes.size();
	int colN;
	int rowN;
	for(int i = 0; i < sizeHexes; i++){
		 colN = hexes[i]->GetHexColumn();
		 rowN = hexes[i]->GetHexRow();
		 mapBuffer[colN + 16*rowN] = colorMod;
	}
}


void mapHexesToMap(std::vector<Hexagon*> hexes, GameMap *map, HexagonType type){
	int sizeHexes = hexes.size();
	for(int i = 0; i < sizeHexes; i++){
		 map->ChangeHex(hexes[i]->GetHexRow(), hexes[i]->GetHexColumn(), type);
	}
}



void mapToBuffer(GameMap *map, uint8_t* mapBuffer) {

	for(int row = 0; row < map->GetRows(); row++){
		for(int col = 0; col < map->GetColumns(); col++){
			switch(map->GetHex(row, col)->GetType()){
			case BaseHex:
				mapBuffer[col + (row*16)] = 0;
				break;

			case WallHex:
				mapBuffer[col + (row*16)] = 1;
				break;

			case PlayerHex:
				mapBuffer[col + (row*16)] = 2;
				break;

			case MonsterHex:
				mapBuffer[col + (row*16)] = 3;
				break;


			case ChestHex:
				mapBuffer[col + (row*16)] = 4;
				break;


			case MoveHex:
				mapBuffer[col + (row*16)] = 5;
				break;

//			case PlayerHexTurn:
//				mapBuffer[col + (row*16)] = 6;
//				break;


			}
		}
	}

}


void bufferToMap(GameMap *map,uint8_t* mapBuffer) {

	for(int row = 0; row < map->GetRows(); row++){
		for(int col = 0; col < map->GetColumns(); col++){
			switch(mapBuffer[col + (row*16)]){
			case 0:
				map->ChangeHex(row, col, BaseHex);
				break;

			case 1:
				map->ChangeHex(row, col, WallHex);
				break;

			case 2:
				map->ChangeHex(row, col, PlayerHex);
				break;

			case 3:
				map->ChangeHex(row, col, MonsterHex);
				break;

			case 4:
				map->ChangeHex(row, col, ChestHex);
				break;

			case 5:
				map->ChangeHex(row, col, MoveHex);
				break;

//			case 6:
//				map->ChangeHex(row, col, PlayerHexTurn);
//				break;


			}



		}
	}
}



GameMap* movementMode(TIM_HandleTypeDef htim1, TIM_HandleTypeDef htim3,MCP23017_HandleTypeDef hmcps1[8], MCP23017_HandleTypeDef hmcps2[8], GameMap *map, Hexagon* currHex, Character* _character, int* _movement){
	  TIM_HandleTypeDef timers[] = {htim1, htim3};
	  uint8_t mapBuffer[256];
	  mapToBuffer(map, mapBuffer);
//	  uint8_t prevMapBuffer[256];
//	  std::memcpy(prevMapBuffer, mapBuffer, sizeof(uint8_t) * 256);

	  int movement = *_movement;
	  //Hexagon* currHex = map->GetHex(0,1);
	  std::vector<Hexagon*> possibleMoves = map->PossibleMovements(currHex, movement);//but get character hex and character movement score
	  mapHexesToBuffer(mapBuffer, possibleMoves, MoveHex);

	  //clearMap(htim1,htim3);
	  displayMap(htim1, htim3, mapBuffer, sizeof(mapBuffer)/sizeof(uint8_t));
	  //displayMap(htim1, htim3, mapBuffer, sizeof(mapBuffer)/sizeof(uint8_t));
	//  std::memcpy(mapBuffer, prevMapBuffer, sizeof(uint8_t) * 256); //set mapBuffer back to default

	  key = '\0';

	  LCD_FillScreen(LCD_WHITE);
	  LCD_WriteString(15, 50, "MOVE PIECE", FONT, LCD_BLACK, LCD_WHITE);
	  HAL_Delay(2000);


	  while (1) {
		  if (key == '#') {
			  key = '\0';

			  for(int hex = 0; hex < (int)possibleMoves.size(); hex++){
				  int row = possibleMoves[hex]->GetHexRow();
				  int col = possibleMoves[hex]->GetHexColumn();

				  bool hallTrig = checkHallSensor(row, col, hmcps1, hmcps2);

				  if(hallTrig){
					  //update buffer or map
					  mapBuffer[col + 16*row] = currHex->GetType();
					  _character->SetColumn(col);
					  _character->SetRow(row);
					  mapBuffer[currHex->GetHexColumn() + 16*currHex->GetHexRow()] = BaseHex;
					  movement = movement - map->HexDistance(currHex, possibleMoves[hex]);
					  map->GetHex(row,col)->SetType(currHex->GetType());
					  currHex->SetType(BaseHex);
					  currHex->SetPassable(true);
					  currHex = map->GetHex(row,col);
					  currHex->SetPassable(false);


					  //std::memcpy(prevMapBuffer, mapBuffer, sizeof(uint8_t) * 256);
					  if(movement > 0){
						  possibleMoves = map->PossibleMovements(currHex, movement);
						  mapHexesToBuffer(mapBuffer, possibleMoves, MoveHex);
						  displayMap(htim1, htim3, mapBuffer, sizeof(mapBuffer)/sizeof(uint8_t));
						 // std::memcpy(mapBuffer, prevMapBuffer, sizeof(uint8_t) * 256); //set mapBuffer back to default
					  }
					  else{
						  displayMap(htim1, htim3, mapBuffer, sizeof(mapBuffer)/sizeof(uint8_t));
						  break;
					  }
				  }

			  }

			  if(movement <= 0){
				  displayMap(htim1, htim3, mapBuffer, sizeof(mapBuffer)/sizeof(uint8_t));
				  break;
			  }

		  }
		  if(key == '*'){
			  key = '\0';
			  displayMap(htim1, htim3, mapBuffer, sizeof(mapBuffer)/sizeof(uint8_t));
			  break;
		  }

	  }

	  bufferToMap(map,mapBuffer);
	  *_movement = movement;

	  return map;
}



void clearMap(TIM_HandleTypeDef htim1, TIM_HandleTypeDef htim3){

	for(int i= 0; i < 32; i++){
		Set_LED(i,0,0,0);
	}
	for(int ch = 1; ch < 5; ch++){
	  Set_Brightness(20);
	  WS2812_Send(&htim1,ch);
	}
	for(int ch = 1; ch < 5; ch++){
	  Set_Brightness(20);
	  WS2812_Send(&htim3,ch);
	}
}


GameMap* combatMode(TIM_HandleTypeDef htim1, TIM_HandleTypeDef htim3,MCP23017_HandleTypeDef hmcps1[8], MCP23017_HandleTypeDef hmcps2[8], GameMap *map, Hexagon* currHex){
	return map;
}


void blinkLED(uint8_t* mapCharBuffer , int row, int col, int type){
	displayMap(htim1, htim3, mapCharBuffer, 256);
	HAL_Delay(500);
	mapCharBuffer[col + 16 * row] = type;
	displayMap(htim1, htim3, mapCharBuffer, 256);
	mapCharBuffer[col + 16 * row] = BaseHex;
	HAL_Delay(500);

}


void FOVToBuffer(uint8_t* mapBuffer, std::vector<Hexagon*> hexes){
	int sizeHexes = hexes.size();
	int colN;
	int rowN;
	for(int i = 0; i < sizeHexes; i++){
		 colN = hexes[i]->GetHexColumn();
		 rowN = hexes[i]->GetHexRow();
		 mapBuffer[colN + 16*rowN] = hexes[i]->GetType();
		// mapBuffer[colN + 16*rowN] = 1;
	}
}




