#include "states.h"


extern GameState game_state;
extern char key;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;

extern GameMap* map;
extern GameCharacters * characters;

extern MCP23017_HandleTypeDef hmcps1[8];
extern MCP23017_HandleTypeDef hmcps2[8];

void Welcome(void) {
	int counter = 0;
	key = '\0';
	LCD_FillScreen(LCD_WHITE);
	LCD_WriteString(15, 50, "The Dungeon Crawler", FONT, LCD_BLACK, LCD_WHITE);
	HAL_Delay(2000);
	while (1) {
		if (key == '#') {
			key = '\0';
			game_state = MENU_STATE;
			LCD_FillScreen(LCD_WHITE);
			HAL_Delay(500);
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
}

void Menu(void) {
	int selection = 1;
	int prev_selection = 0;
	int y_pos = 50;
	key = '\0';
	LCD_WriteStringCentered(50, "DM Mode", FONT, LCD_BLACK, LCD_WHITE);
	LCD_WriteStringCentered(100, "Playing Mode", FONT, LCD_BLACK, LCD_WHITE);
	LCD_FillRectangle(10, selection * y_pos, 10, 18, LCD_BLACK);
	while (1) {
		if (key == '#') {
			key = '\0';
			switch (selection) {
				case (1):
					game_state = DM_MODE_STATE;
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
	LCD_WriteStringCentered(50, "Upload Map", FONT, LCD_BLACK, LCD_WHITE);
	LCD_WriteStringCentered(100, "View Map", FONT, LCD_BLACK, LCD_WHITE);
	LCD_WriteStringCentered(150, "Return To Menu", FONT, LCD_BLACK, LCD_WHITE);
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

void Upload_Map(void) {
	int usb_status = check_usb_connection();
	if(usb_status) {
		LCD_WriteStringCentered(100, "Waiting for USB", FONT, LCD_BLACK, LCD_WHITE);
		LCD_WriteStringCentered(90, "Connection...", FONT, LCD_BLACK, LCD_WHITE);

	}

	while (check_usb_connection());
	LCD_FillScreen(LCD_WHITE);
	LCD_WriteStringCentered(100, "Send Map Now", FONT, LCD_BLACK, LCD_WHITE);

	if (load_map() != 0) {
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

	int selection = 1;
	int prev_selection = 0;
	int y_pos = 50;
	key = '\0';
	LCD_FillScreen(LCD_WHITE);
	HAL_Delay(500);
	LCD_WriteStringCentered(100, "Map Uploaded", FONT, LCD_BLACK, LCD_WHITE);
	HAL_Delay(500);
	LCD_FillScreen(LCD_WHITE);
	LCD_WriteStringCentered(50, "View Map", FONT, LCD_BLACK, LCD_WHITE);
	LCD_WriteStringCentered(100, "Return To Menu", FONT, LCD_BLACK, LCD_WHITE);
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
	if (map == NULL) {
		game_state = DM_MODE_STATE;
		LCD_WriteStringCentered(100, "Map Not Initialized", FONT, LCD_WHITE, LCD_BLACK);
		LCD_FillScreen(LCD_WHITE);
		HAL_Delay(500);

		return;
	}
	uint8_t mapBuffer[256];

	if(map == NULL) {
		game_state = DM_MODE_STATE;
		LCD_WriteStringCentered(100, "Map not Initialized", FONT, LCD_WHITE, LCD_BLACK);
		HAL_Delay(500);
		LCD_FillScreen(LCD_WHITE);
		return;
	}

	for(int row = 0; row < map->GetRows(); row++){
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

	if(characters == NULL || map == NULL) {
		game_state = MENU_STATE;
		LCD_WriteStringCentered(100, "Characters and/or Map", FONT, LCD_BLACK, LCD_WHITE);
		LCD_WriteStringCentered(90, "not Initialized", FONT, LCD_BLACK, LCD_WHITE);

		HAL_Delay(500);
		return;
	}
	uint8_t mapCharBuffer[256];
	uint8_t mapBuffer[256];
	memset(mapCharBuffer, 0, sizeof(mapCharBuffer));
	mapToBuffer(map, mapBuffer);
    for (int i = 0; i < characters->GetNumberCharacters(); i++) {
    	Character* character = characters->GetCharacter(i);
    	if (character->GetCharacterType() == Player) {
    		std::pair<int, int> position = character->GetPosition();
    		LCD_WriteStringCentered(10, "Place Token For", FONT, LCD_BLACK, LCD_WHITE);
    		std::string name = character->GetName();
    		const char* char_name = name.c_str();
    		LCD_WriteStringCentered(50, char_name, FONT, LCD_BLACK, LCD_WHITE);



    		int start_tick = HAL_GetTick();
    		while (1) {
    			int cur_tick = HAL_GetTick();
				if ((cur_tick - start_tick) >= 60000) {
					return;
				}


				blinkLED(mapCharBuffer , position.first, position.second, PlayerHex);


    			bool hallTrig = checkHallSensor(position.first, position.second, hmcps1, hmcps2);
    			if (hallTrig) {
    				mapBuffer[position.second + 16  *position.first] = PlayerHex;
    				LCD_FillScreen(LCD_WHITE);
    				HAL_Delay(500);
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
					LCD_FillScreen(LCD_WHITE);
					switch (selection) {
						case (1): {
							LCD_WriteStringCentered(50, "Do not remove token", FONT, LCD_BLACK, LCD_WHITE);
							HAL_Delay(1000);
							LCD_FillScreen(LCD_WHITE);
							LCD_WriteStringCentered(10, "Insert initiative", FONT, LCD_BLACK, LCD_WHITE);
							LCD_WriteStringCentered(30, "roll", FONT, LCD_BLACK, LCD_WHITE);
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
							        LCD_FillScreen(LCD_WHITE);
							        HAL_Delay(500);
							        break;
							    }
							    else if (key == '*') {
							        key = '\0';
							        if (no_character > 0) {
							        	LCD_WriteStringCentered(100, initiative, FONT, LCD_WHITE, LCD_WHITE);
							            no_character--;
							            initiative[no_character] = '\0';
							            LCD_WriteStringCentered(100, initiative, FONT, LCD_BLACK, LCD_WHITE);
							        }
							    }
							    else if (no_character < 2 && Key_Is_Number(key)) {
							    	LCD_WriteStringCentered(100, initiative, FONT, LCD_WHITE, LCD_WHITE);
							        initiative[no_character] = key;
							        no_character++;
							        initiative[no_character] = '\0';
							        LCD_WriteStringCentered(100, initiative, FONT, LCD_BLACK, LCD_WHITE);
							        key = '\0';
							    }
							}

				    		selection = 1;
				    		prev_selection = 0;
				    		y_pos = 50;
				    		key = '\0';
							LCD_WriteStringCentered(50, "Check Stats", FONT, LCD_BLACK, LCD_WHITE);
							LCD_WriteStringCentered(100, "Continue", FONT, LCD_BLACK, LCD_WHITE);
							LCD_FillRectangle(10, selection * y_pos, 10, 18, LCD_BLACK);
							while (1) {
								if (key == '#') {
									key = '\0';
									switch (selection) {
										case (1):
											View_Character_Info(character);
										case (2):
											break;
									}
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
				HAL_Delay(100);
				int cur_tick = HAL_GetTick();
				if ((cur_tick - start_tick) >= 60000) {
					return;
				}
				if (key == '#' && no_character != 0) {
					key = '\0';
					initiative[no_character] = '\0';
					character->SetInitiative(atoi(initiative));
			        LCD_FillScreen(LCD_WHITE);
			        HAL_Delay(500);
					break;
				}
				else if (key == '*') {
					key = '\0';
					if (no_character > 0) {
						LCD_WriteStringCentered(100, initiative, FONT, LCD_WHITE, LCD_WHITE);
						no_character--;
						initiative[no_character] = '\0';
						LCD_WriteStringCentered(100, initiative, FONT, LCD_BLACK, LCD_WHITE);
					}
				}
				else if (no_character < 2 && Key_Is_Number(key)) {
					LCD_WriteStringCentered(100, initiative, FONT, LCD_WHITE, LCD_WHITE);
					initiative[no_character] = key;
					no_character++;
					initiative[no_character] = '\0';
					LCD_WriteStringCentered(100, initiative, FONT, LCD_BLACK, LCD_WHITE);
				}
			}

    		int selection = 1;
    		int prev_selection = 0;
    		int y_pos = 50;
    		key = '\0';
			LCD_WriteStringCentered(50, "Check Stats", FONT, LCD_BLACK, LCD_WHITE);
			LCD_WriteStringCentered(100, "Continue", FONT, LCD_BLACK, LCD_WHITE);
			LCD_FillRectangle(10, selection * y_pos, 10, 18, LCD_BLACK);
			while (1) {
				if (key == '#') {
					key = '\0';
					switch (selection) {
						case (1):
							View_Character_Info(character);
						case (2):
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
    }
    characters->SortCharacters();


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
					game_state = GAME_START_STATE;
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

	displayMap(htim1, htim3, mapBuffer, sizeof(mapBuffer) / sizeof(uint8_t));
}

void Game_Loop(void) {
	int selection = 1;
	int prev_selection = 0;
	int y_pos = 25;
	key = '\0';
	if(characters == NULL) {
		game_state = MENU_STATE;
		LCD_WriteStringCentered(100, "Characters not Initialized", FONT, LCD_BLACK, LCD_WHITE);
		HAL_Delay(500);
		LCD_FillScreen(LCD_WHITE);
		return;
	}
	while(characters->GetNumberCharacters() > 1) {
		for(int i = 0; i < characters->GetNumberCharacters(); i++) {
			//check conditions for continuing game
			if(map == NULL) {
				game_state = MENU_STATE;
				LCD_WriteStringCentered(100, "Map not Initialized", FONT, LCD_BLACK, LCD_WHITE);
				HAL_Delay(500);
				LCD_FillScreen(LCD_WHITE);
				return;
			}

			//Display character name ex: it is neils turn
			y_pos = 25;
			LCD_WriteStringCentered(40, "It is", FONT, LCD_BLACK, LCD_WHITE);
			LCD_WriteStringCentered(60, (characters->GetCharacter(i)->GetName() + "'s").c_str(), FONT, LCD_BLACK, LCD_WHITE);
			LCD_WriteStringCentered(80, "Turn", FONT, LCD_BLACK, LCD_WHITE);

			LCD_WriteStringCentered(200, "Continue", FONT, LCD_BLACK, LCD_WHITE);
			LCD_WriteStringCentered(225, "Return to Menu", FONT, LCD_BLACK, LCD_WHITE);
			LCD_FillRectangle(10, 175+ selection * y_pos, 10, 18, LCD_BLACK);

			while (1) {
				if (key == '#') {
					key = '\0';
					switch (selection) {
						case (1):
							break;
						case (2):
							game_state = MENU_STATE;
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
					LCD_FillRectangle(10, 175 +prev_selection * y_pos, 10, 18, LCD_WHITE);
					LCD_FillRectangle(10, 175 + selection * y_pos, 10, 18, LCD_BLACK);
					prev_selection = selection;
				}
			}

			y_pos = 20;
			selection = 1;
			HAL_Delay(500);

			int movement = 3;
			int action = 0;

			while(movement > 0 || action == 1){

				//Give option for info, action, move
				LCD_WriteStringCentered(50, "How will you proceed?", FONT, LCD_BLACK, LCD_WHITE);
				LCD_WriteStringCentered(100, "Move Player", FONT, LCD_BLACK, LCD_WHITE);
				LCD_WriteStringCentered(120, "Character Info", FONT, LCD_BLACK, LCD_WHITE);
				LCD_WriteStringCentered(140, "Enter Combat", FONT, LCD_BLACK, LCD_WHITE); //maybe add conditional here for if combat is possible
				LCD_WriteStringCentered(160, "End Turn", FONT, LCD_BLACK, LCD_WHITE);
				LCD_FillRectangle(10, 80 +selection * y_pos, 10, 18, LCD_BLACK);

				//branch based on selection
				key = '\0';
				while (1) {
					if (key == '#') {
						key = '\0';
						switch (selection) {
							case (1):
								//move function here
								//map = movementMode(htim1, htim3,hmcps1, hmcps2, map, map->GetHex(characters->GetCharacter(i)->GetRow(), characters->GetCharacter(i)->GetColumn()), &movement);
								map = movementMode(htim1, htim3,hmcps1, hmcps2, map, map->GetHex(characters->GetCharacter(i)->GetRow(), characters->GetCharacter(i)->GetColumn()),characters->GetCharacter(i), &movement);

								break;
							case (2):
								View_Character_Info(characters->GetCharacter(i));
								break;
							case (3):
								//combat function here
								action = 0;
								break;

							case (4):
								//end turn
								movement = 0;
								action = 0;
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
						selection = (selection < 4) ? selection + 1 : 4;
					}
					if (selection != prev_selection) {
						LCD_FillRectangle(10, 80 +prev_selection * y_pos, 10, 18, LCD_WHITE);
						LCD_FillRectangle(10, 80 +selection * y_pos, 10, 18, LCD_BLACK);
						prev_selection = selection;
					}
				}
			}

		}
	}
	if(characters->GetNumberCharacters() == 1) {
		LCD_WriteStringCentered(80, "Congrats", FONT, LCD_BLACK, LCD_WHITE);
		LCD_WriteStringCentered(100, characters->GetCharacter(0)->GetName().c_str(), FONT, LCD_BLACK, LCD_WHITE);
		LCD_WriteStringCentered(120, "You win!", FONT, LCD_BLACK, LCD_WHITE);

		LCD_WriteStringCentered(200, "Enter to Continue", FONT, LCD_BLACK, LCD_WHITE);
		while(1) {
			if(key == '#') {
				break;
			}
		}

	}
	game_state = MENU_STATE;
}

