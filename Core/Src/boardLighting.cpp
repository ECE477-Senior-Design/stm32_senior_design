//MOST RECENT AUTHOR: Landon
//DATE: 4/3/2024


#include <ws2812b.h>
#include "boardLighting.h"
#include <cstddef>
#include "stm32f4xx_hal.h"




void displayMap(TIM_HandleTypeDef htim1, TIM_HandleTypeDef htim3,uint8_t* mapBuffer, size_t bufferSize ){
	//DISPLAY MAP
	TIM_HandleTypeDef timers[] = {htim1, htim3}; //, htim3, htim4, htim5};4,1,3,5
	int test[256] = {};
	uint32_t color;

	int channels[] = {4,3,2,1, 4, 3, 2, 1}; //only 4 because groups of two so only run 4 times while operating with half of board
	for(int pcb = 0; pcb < ((int)bufferSize / MAX_LED); pcb++){
		for(int led = 0; led < MAX_LED; led++){
			color = ((LED_Data[led][1]<<16) | (LED_Data[led][2]<<8) | (LED_Data[led][3]));
		  switch(mapBuffer[(MAX_LED*pcb) + led]){
			  case 0:
				  Set_LED(led,0,0,0); //nothing/floor
				  test[led + (pcb*32)] = 0;
				  break;

			  case 1:
				  Set_LED(led,50, 50, 50); //wall
				  test[led + (pcb*32)] = 1;
				  break;

			  case 2:
				 Set_LED(led,0,0,255); //character/player
				 test[led + (pcb*32)] = 2;
				  break;

			 case 3:
				 Set_LED(led,255,0,0); //enemy
				 test[led + (pcb*32)] = 3;
				  break;

			 case 4:
				 Set_LED(led,100,100,0); //chest
				 test[led + (pcb*32)] = 4;
				  break;

			 default:
				 Set_LED(led,0,0,0); //default off
				 test[led + (pcb*32)] = 5;
				  break;

		  }
		}
		int timer = pcb / 4;
		Set_Brightness(20);
		WS2812_Send(&(timers[timer]),channels[pcb]);
	}
}



void mapHexes(uint8_t* mapBuffer, uint8_t* mapBufferPrev, std::vector<Hexagon*> hexes, int colorMod){
	std::memcpy(mapBufferPrev, mapBuffer, sizeof(uint8_t) * 256);
	int sizeHexes = hexes.size();
	int colN;
	int rowN;
	for(int i = 0; i < sizeHexes; i++){
		 colN = hexes[i]->GetHexColumn();
		 rowN = hexes[i]->GetHexRow();
		 mapBuffer[colN + 16*rowN] = colorMod;
	}
}














