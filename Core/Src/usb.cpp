
#include "stm32f4xx_hal.h"
#include "GameMap.h"
#include "GameCharacters.h"
#include "usb.h"


uint8_t buffer[256];
int headPos = 0;
int tempPos = 0;
int charSend = 0;
int startTick;
int curTick;

extern GameMap *map;
extern GameCharacters *characters;
extern USBD_HandleTypeDef hUsbDeviceFS;

int load_map(void) {

	startTick = HAL_GetTick();
	int waitTime = 20; //wait time in seconds

	//get map
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

	headPos = 0;
	charSend = 1;

	std::vector<std::string> characterStrings;

	while(1)
	{
		memset(buffer, 0, 256);
		USBD_CDC_ReceivePacket(&hUsbDeviceFS);
		HAL_Delay(100);
		if(buffer[0] == 0) //check for empty buffer, no receive
		{
			break;
		}
		else
		{
			std::string test = std::string((char *)buffer);
			test.resize(headPos);

			characterStrings.push_back(test);
			headPos = 0;
			//USBD_CDC_ReceivePacket(&hUsbDeviceFS);
		}
	}


	characters = new GameCharacters(characterStrings);

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
