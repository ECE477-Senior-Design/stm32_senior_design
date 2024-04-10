
#include "stm32f4xx_hal.h"
#include "GameMap.h"
#include "GameCharacters.h"

uint8_t buffer[256];
int sent_flag = 0;
char gs[256];
int headPos = 0;
int startTick;
int curTick;
int readchars = 0;
std::vector<std::string> character_str;

extern GameMap *map;
extern GameCharacters *characters;

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

	std::string gamestring = std::string((char *)buffer);


	map = new GameMap(gamestring);

	return 0;
}

int check_usb_connection(void) {
	GPIO_PinState state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9);
	if(state == GPIO_PIN_RESET) {
		return -1;
	}
	else {
		return 0;
	}
}
