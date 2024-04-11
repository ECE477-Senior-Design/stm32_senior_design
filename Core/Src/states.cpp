/*
 * states.c
 *
 *  Created on: Apr 4, 2024
 *      Author: neilt
 */

#include "ws2812b.h"
#include "states.h"
#include "main.h"
#include "lcd.h"
#include "keypad.h"
#include "usb.h"
#include "boardLighting.h"

extern GameState game_state;
extern char key;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;

extern MCP23017_HandleTypeDef hmcps1[8];
extern MCP23017_HandleTypeDef hmcps2[8];

extern GameMap* map;
extern GameCharacters* characters;

void LED_Test(void) {
	HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_2);
	HAL_Delay(500);
	HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_3);
	HAL_Delay(500);
	HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_4);
	HAL_Delay(500);
	HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_5);
	HAL_Delay(500);
	HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_6);
	HAL_Delay(500);
}

void Welcome(void) {
	int counter = 0;
	key = '\0';
	LCD_FillScreen(LCD_WHITE);
	LCD_WriteString(15, 50, "The Dungeon Crawler", FONT, LCD_BLACK, LCD_WHITE);
	HAL_Delay(2000);
	while (1) {
		if (key == '#') {
			key = '\0';
			break;
		}
		if (counter % 2 == 0) {
			LCD_WriteString(26, 200, "Enter The Dungeon", FONT, LCD_BLACK, LCD_WHITE);
		}
		else {
			LCD_FillRectangle(0, 200, 240, 18, LCD_WHITE);
		}

		HAL_Delay(500);
		counter++;
	}
	game_state = MENU_STATE;
	LCD_FillScreen(LCD_WHITE);
	HAL_Delay(500);
}

void Menu(void) {
	int selection = 1;
	int prev_selection = 0;
	int y_pos = 50;
	key = '\0';
	LCD_WriteString(81, 50, "DM Mode", FONT, LCD_BLACK, LCD_WHITE);
	LCD_WriteString(54, 100, "Playing Mode", FONT, LCD_BLACK, LCD_WHITE);
	LCD_FillRectangle(10, selection * y_pos, 10, 18, LCD_BLACK);
	while (1) {
		if (key == '#') {
			key = '\0';
			switch (selection) {
				case (1):
					game_state =DM_MODE_STATE;
					break;
				case (2):
					game_state = PLAYING_MODE_STATE;
					break;
			}
			LCD_FillScreen(LCD_WHITE);
			HAL_Delay(500);
			break;
		}
		if (key == 'A') {
			key = '\0';
			selection = (selection > 1) ? selection - 1 : 1;
		}
		if (key == 'D') {
			key = '\0';
			selection = (selection < 2) ? selection + 1 : 2;
		}

        if (selection != prev_selection) {
            LCD_FillRectangle(10, prev_selection * y_pos, 10, 18, LCD_WHITE);
            LCD_FillRectangle(10, selection * y_pos, 10, 18, LCD_BLACK);
            prev_selection = selection;
        }
	}
}

void DM_Mode(void) {
	int selection = 1;
	int prev_selection = 0;
	int y_pos = 50;
	key = '\0';
	LCD_WriteString(65, 50, "Upload Map", FONT, LCD_BLACK, LCD_WHITE);
	LCD_WriteString(76, 100, "View Map", FONT, LCD_BLACK, LCD_WHITE);
	LCD_WriteString(48, 150, "Return to Menu", FONT, LCD_BLACK, LCD_WHITE);
	LCD_FillRectangle(10, selection * y_pos, 10, 18, LCD_BLACK);
	while (1) {
		if (key == '#') {
			key = '\0';
			switch (selection) {
				case (1):
					game_state = UPLOAD_MAP_STATE;
					break;
				case (2):
					game_state = VIEW_MAP_STATE;
					break;
				case (3):
					game_state = MENU_STATE;
					break;
			}
			LCD_FillScreen(LCD_WHITE);
			HAL_Delay(500);
			break;
		}
		if (key == 'A') {
			key = '\0';
		    selection = (selection > 1) ? selection - 1 : 1;
		}
		if (key == 'D') {
			key = '\0';
		    selection = (selection < 3) ? selection + 1 : 3;
		}

		if (selection != prev_selection) {
			LCD_FillRectangle(10, prev_selection * y_pos, 10, 18, LCD_WHITE);
			LCD_FillRectangle(10, selection * y_pos, 10, 18, LCD_BLACK);
			prev_selection = selection;
		}
	}
}

void Upload_Map(void)
{
	int selection = 1;
	int prev_selection = 0;
	int y_pos = 50;
	key = '\0';
	LCD_WriteStringCentered(100, "Send Map Now", FONT, LCD_BLACK, LCD_WHITE);

	if (load_map() != 0){
		LCD_FillScreen(LCD_WHITE);
		LCD_WriteStringCentered(100, "Map Send Timeout", FONT, LCD_BLACK, LCD_WHITE);
		HAL_Delay(1000);
		LCD_WriteStringCentered(150, "Returning to Menu", FONT, LCD_BLACK, LCD_WHITE);
		HAL_Delay(1000);
		LCD_FillScreen(LCD_WHITE);
		HAL_Delay(500);
		game_state = MENU_STATE;

		return;
	}

	LCD_FillScreen(LCD_WHITE);
	HAL_Delay(500);
	LCD_WriteStringCentered(100, "Map Uploaded", FONT, LCD_BLACK, LCD_WHITE);
	HAL_Delay(500);
	LCD_FillScreen(LCD_WHITE);
	LCD_WriteStringCentered(50, "View Map", FONT, LCD_BLACK, LCD_WHITE);
	LCD_WriteStringCentered(100, "Return to Menu", FONT, LCD_BLACK, LCD_WHITE);
	LCD_FillRectangle(10, selection * y_pos, 10, 18, LCD_BLACK);
	while (1) {
		if (key == '#') {
			key = '\0';
			switch (selection) {
				case (1):
					game_state = VIEW_MAP_STATE;
					break;
				case (2):
					game_state = MENU_STATE;
					break;
			}
			LCD_FillScreen(LCD_WHITE);
			HAL_Delay(500);
			break;
		}
		if (key == 'A') {
			key = '\0';
		    selection = (selection > 1) ? selection - 1 : 1;
		}
		if (key == 'D') {
			key = '\0';
		    selection = (selection < 2) ? selection + 1 : 2;
		}

		if (selection != prev_selection) {
			LCD_FillRectangle(10, prev_selection * y_pos, 10, 18, LCD_WHITE);
			LCD_FillRectangle(10, selection * y_pos, 10, 18, LCD_BLACK);
			prev_selection = selection;
		}
	}
}

void View_Map() {
	uint8_t mapBuffer[256];
	for (int row = 0; row < map->GetRows(); row++){
		for(int col = 0; col < map->GetColumns(); col++){
			switch(map->GetHex(row, col)->GetType()){
				case BaseHex:
					mapBuffer[col + (row * 16)] = 0;
					break;
				case WallHex:
					mapBuffer[col + (row * 16)] = 1;
					break;
				case PlayerHex:
					mapBuffer[col + (row * 16)] = 2;
					break;
				case MonsterHex:
					mapBuffer[col + (row * 16)] = 3;
					break;
				case ChestHex:
					mapBuffer[col + (row * 16)] = 4;
					break;
				case MoveHex:
					mapBuffer[col + (row * 16)] = 5;
					break;
			}
		}
	}
	displayMap(htim1, htim3, mapBuffer, sizeof(mapBuffer) / sizeof(uint8_t));
	game_state = MENU_STATE;
}

void Playing_Mode() {
	uint8_t mapCharBuffer[256];
	uint8_t mapBuffer[256];
	memset(mapCharBuffer, 0, sizeof(mapCharBuffer));
	mapToBuffer(map, mapBuffer);
    for (int i = 0; i < characters->GetNumberCharacters(); i++) {
    	Character* character = characters->GetCharacter(i);
    	if (character->GetCharacterType() == Player) {
    		std::pair<int, int> position = character->GetPosition();
    		LCD_WriteStringCentered(10, "Place token for", FONT, LCD_BLACK, LCD_WHITE);
    		std::string name = character->GetName();
    		const char* char_name = name.c_str();
    		LCD_WriteStringCentered(50, char_name, FONT, LCD_BLACK, LCD_WHITE);

    		mapCharBuffer[position.second + 16 * position.first] = PlayerHex;
    		displayMap(htim1, htim3, mapCharBuffer, sizeof(mapCharBuffer) / sizeof(uint8_t));
    		mapCharBuffer[position.second + 16 * position.first] = BaseHex;

    		int start_tick = HAL_GetTick();
    		while (1) {
    			int cur_tick = HAL_GetTick();
				if ((cur_tick - start_tick) >= 60000) {
					return;
				}
    			bool hallTrig = checkHallSensor(position.first, position.second, hmcps1, hmcps2);

    			if (hallTrig) {
    				mapBuffer[position.second + 16  *position.first] = PlayerHex;
    				break;
    			}
    		}

    		int selection = 1;
    		int prev_selection = 0;
    		int y_pos = 50;
    		key = '\0';
    		LCD_FillScreen(LCD_WHITE);
    		LCD_WriteStringCentered(50, "Confirm", FONT, LCD_BLACK, LCD_WHITE);
			LCD_WriteStringCentered(100, "Retry", FONT, LCD_BLACK, LCD_WHITE);
			LCD_FillRectangle(10, selection * y_pos, 10, 18, LCD_BLACK);
			while (1) {
				if (key == '#') {
					key = '\0';
					switch (selection) {
						case (1): {
							LCD_FillScreen(LCD_WHITE);
							LCD_WriteStringCentered(50, "Do not remove token", FONT, LCD_BLACK, LCD_WHITE);
							HAL_Delay(1000);
							LCD_FillScreen(LCD_WHITE);
							LCD_WriteStringCentered(10, "Insert initiative", FONT, LCD_BLACK, LCD_WHITE);
							LCD_WriteStringCentered(30, "roll", FONT, LCD_BLACK, LCD_WHITE);
							key = '\0';
							char* initiative = new char[3];
							int no_character = 0;
							int start_tick = HAL_GetTick();
							while (1) {
								int cur_tick = HAL_GetTick();
								if ((cur_tick - start_tick) >= 60000) {
									return;
								}
							    if (key == '#' && no_character != 0) {
							        key = '\0';
							        initiative[no_character] = '\0';
							        character->SetInitiative(atoi(initiative));
							        break;
							    }
							    else if (key == '*') {
							        key = '\0';
							        if (no_character > 0) {
							        	LCD_WriteStringCentered(50, initiative, FONT, LCD_WHITE, LCD_WHITE);
							            no_character--;
							            initiative[no_character] = '\0';
							            LCD_WriteStringCentered(50, initiative, FONT, LCD_BLACK, LCD_WHITE);
							        }
							    }
							    else if (no_character < 2 && Key_Is_Number(key)) {
							    	LCD_WriteStringCentered(50, initiative, FONT, LCD_WHITE, LCD_WHITE);
							        initiative[no_character] = key;
							        no_character++;
							        initiative[no_character] = '\0';
							        LCD_WriteStringCentered(50, initiative, FONT, LCD_BLACK, LCD_WHITE);
							        key = '\0';
							    }
							}
							break;
						}
						case (2):
							i--;
							break;
					}
					LCD_FillScreen(LCD_WHITE);
					HAL_Delay(500);
					break;
				}
				if (key == 'A') {
					key = '\0';
					selection = (selection > 1) ? selection - 1 : 1;
				}
				if (key == 'D') {
					key = '\0';
					selection = (selection < 2) ? selection + 1 : 2;
				}

				if (selection != prev_selection) {
					LCD_FillRectangle(10, prev_selection * y_pos, 10, 18, LCD_WHITE);
					LCD_FillRectangle(10, selection * y_pos, 10, 18, LCD_BLACK);
					prev_selection = selection;
				}
			}
    	}
    	else {
    		LCD_WriteStringCentered(10, "Insert initiative", FONT, LCD_BLACK, LCD_WHITE);
    		LCD_WriteStringCentered(30, "roll for", FONT, LCD_BLACK, LCD_WHITE);
    		std::string name = character->GetName();
    		const char* char_name = name.c_str();
    		LCD_WriteStringCentered(50, char_name, FONT, LCD_BLACK, LCD_WHITE);
			key = '\0';
			char* initiative = new char[3];
			int no_character = 0;
			int start_tick = HAL_GetTick();
			while (1) {
				int cur_tick = HAL_GetTick();
				if ((cur_tick - start_tick) >= 60000) {
					return;
				}
				if (key == '#' && no_character != 0) {
					key = '\0';
					initiative[no_character] = '\0';
					character->SetInitiative(atoi(initiative));
					break;
				}
				else if (key == '*') {
					key = '\0';
					if (no_character > 0) {
						LCD_WriteStringCentered(50, initiative, FONT, LCD_WHITE, LCD_WHITE);
						no_character--;
						initiative[no_character] = '\0';
						LCD_WriteStringCentered(50, initiative, FONT, LCD_BLACK, LCD_WHITE);
					}
				}
				else if (no_character < 2 && Key_Is_Number(key)) {
					LCD_WriteStringCentered(50, initiative, FONT, LCD_WHITE, LCD_WHITE);
					initiative[no_character] = key;
					no_character++;
					initiative[no_character] = '\0';
					LCD_WriteStringCentered(50, initiative, FONT, LCD_BLACK, LCD_WHITE);
				}
			}
			i++;
    	}
    }

    displayMap(htim1, htim3, mapBuffer, sizeof(mapBuffer) / sizeof(uint8_t));
    game_state = GAME_START_STATE;
}

void Game_Start() {
	int selection = 1;
	int prev_selection = 0;
	int y_pos = 50;
	key = '\0';
	LCD_WriteStringCentered(50, "Start Game", FONT, LCD_BLACK, LCD_WHITE);
	LCD_WriteStringCentered(100, "Return to Menu", FONT, LCD_BLACK, LCD_WHITE);
	LCD_FillRectangle(10, selection * y_pos, 10, 18, LCD_BLACK);
	while (1) {
		if (key == '#') {
			key = '\0';
			switch (selection) {
				case (1):
					break;
				case (2):
					game_state = MENU_STATE;
					LCD_FillScreen(LCD_WHITE);
					HAL_Delay(500);

					return;
			}
			LCD_FillScreen(LCD_WHITE);
			HAL_Delay(500);
			break;
		}
		if (key == 'A') {
			key = '\0';
			selection = (selection > 1) ? selection - 1 : 1;
		}
		if (key == 'D') {
			key = '\0';
			selection = (selection < 2) ? selection + 1 : 2;
		}

		if (selection != prev_selection) {
			LCD_FillRectangle(10, prev_selection * y_pos, 10, 18, LCD_WHITE);
			LCD_FillRectangle(10, selection * y_pos, 10, 18, LCD_BLACK);
			prev_selection = selection;
		}
	}

	characters->SortCharacters();
}
