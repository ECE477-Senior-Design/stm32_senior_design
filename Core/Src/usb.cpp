
#include "stm32f4xx_hal.h"
#include "GameMap.h"

uint8_t buffer[256];
int sent_flag = 0;
char gs[256];
int headPos = 0;
int startTick;
int curTick;

extern GameMap *map;

int load_map(void) {

	startTick = HAL_GetTick();
	int waitTime = 15;

	while(headPos != 256)
	{
		curTick = HAL_GetTick();
		if((curTick - startTick) >= (waitTime * 1000))
		{
			return -1;
		}
	}

	//used these to double check values
	uint8_t secondLast = buffer[254];
	uint8_t last = buffer[255];

	std::string gamestring = std::string((char *)buffer);


	map = new GameMap(gamestring);
	int cols = map->GetColumns();
	int rows = map->GetRows();
	int counter = 0;

	HexagonType checkType;
	for(int i = 0; i < 16; i++)
	{
		for(int j = 0; j < 16; j++)
		{
			checkType = map->GetHex(i,j)->GetType();
			if(checkType == PlayerHex)
			{
				counter++;
			}
		}
	}

	return 0;
}

int check_usb_connection(void) {
	GPIO_PinState state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9);
	if(state) {
		return -1;
	}
	else {
		return 0;
	}
}
