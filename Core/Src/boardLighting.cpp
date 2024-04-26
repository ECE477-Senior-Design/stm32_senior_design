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
#include <random>
#include <ctime>
#include <chrono>

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
extern GameCharacters * characters;
//extern GameCharacters * monsters;


void displayMap(TIM_HandleTypeDef htim1, TIM_HandleTypeDef htim3,uint8_t* mapBuffer, size_t bufferSize ){
	TIM_HandleTypeDef timers[] = {htim1, htim3}; //, htim3, htim4, htim5};4,1,3,5
	int test[256] = {};
	uint32_t color;

	int channels[] = {4,3,2,1, 4, 3, 2, 1}; //only 4 because groups of two so only run 4 times while operating with half of board

	for(int pcb = 0; pcb < ((int)bufferSize / MAX_LED); pcb++){
		for(int led = 0; led < MAX_LED; led++){
			color = ((LED_Data[led][1]<<16) | (LED_Data[led][2]<<8) | (LED_Data[led][3]));
		  switch(mapBuffer[(MAX_LED*pcb) + led]){
			  case BaseHex:
				  Set_LED(led,50,50,55);
				  test[led + (pcb*32)] = 8;
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

			 case 6:  ///PlayerHit
				 Set_LED(led,100, 100, 0); //hex of player when its their turn
				 test[led + (pcb*32)] = 6;
				 break;

			 case 7:  ///MonsterHit
				 Set_LED(led,252, 148, 3); //hex of player when its their turn
				 test[led + (pcb*32)] = 7;
				 break;
			 case 9:  ///DRAGON MOUTH
				 Set_LED(led,245, 0, 100); //hex of player when its their turn
				 test[led + (pcb*32)] = 7;
				 break;

			  case 10:  ///FIRE ORANGE
				 Set_LED(led,255, 60, 0); //hex of player when its their turn
				 test[led + (pcb*32)] = 7;
				 break;

			 default:
				 Set_LED(led,0,0,0); //default off
				 test[led + (pcb*32)] = 0;
				  break;

		  }
		}
		int timer = pcb / 4;
		Set_Brightness(20);
		WS2812_Send(&(timers[timer]), channels[pcb]);
	}
}



void mapHexesToBuffer(uint8_t* mapBuffer, std::vector<Hexagon*> hexes, int colorMod){
	//std::memcpy(mapBufferPrev, mapBuffer, sizeof(uint8_t) * 256);
	int sizeHexes = hexes.size();
	int colN;
	int rowN;
	for (int i = 0; i < sizeHexes; i++) {
		 colN = hexes[i]->GetHexColumn();
		 rowN = hexes[i]->GetHexRow();
		 mapBuffer[colN + 16*rowN] = colorMod;
	}
}

void mapHexesToMap(std::vector<Hexagon*> hexes, GameMap *map, HexagonType type) {
	int sizeHexes = hexes.size();
	for (int i = 0; i < sizeHexes; i++) {
		 map->ChangeHex(hexes[i]->GetHexRow(), hexes[i]->GetHexColumn(), type);
	}
}

void mapToBuffer(GameMap *map, uint8_t* mapBuffer) {
	for (int row = 0; row < map->GetRows(); row++) {
		for (int col = 0; col < map->GetColumns(); col++) {
			switch (map->GetHex(row, col)->GetType()) {
				case BaseHex:
					mapBuffer[col + (row*16)] = 8;
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
				default:
					mapBuffer[col + (row*16)] = 0;
			}
		}
	}
}

void bufferToMap(GameMap* map, uint8_t* mapBuffer) {
	for (int row = 0; row < map->GetRows(); row++) {
		for (int col = 0; col < map->GetColumns(); col++) {
			switch (mapBuffer[col + (row*16)]) {
			case 8:
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
//			default:
//				map->ChangeHex(row, col, 0);
//			case 6:
//				map->ChangeHex(row, col, PlayerHexTurn);
//				break;
			}
		}
	}
}

GameMap* movementMode(TIM_HandleTypeDef htim1, TIM_HandleTypeDef htim3, MCP23017_HandleTypeDef hmcps1[8], MCP23017_HandleTypeDef hmcps2[8], GameMap* map, Hexagon* currHex, Character* character, int* _movement){
	  TIM_HandleTypeDef timers[] = {htim1, htim3};
	  uint8_t mapBuffer[256];
	  mapToBuffer(map, mapBuffer);


	  int movement = *_movement;
	  std::vector<Hexagon*> possibleMoves = map->PossibleMovements(currHex, movement);//but get character hex and character movement score
	  mapHexesToBuffer(mapBuffer, possibleMoves, MoveHex);

	  std::vector<Hexagon*> view = map->FieldOfView(map->GetHex(character->GetRow(), character->GetColumn()), character->GetVisibility());
	  FOVToBuffer(mapBuffer, view);
	  displayMap(htim1, htim3, mapBuffer, sizeof(mapBuffer) / sizeof(uint8_t));

	  LCD_WriteStringCentered(50, "Move Piece", FONT, LCD_BLACK, LCD_WHITE);

	  int y_pos = 50;
	  int selection = 1;
	  int prev_selection = 0;
	  //Give option for info, action, move
	  //LCD_WriteStringCentered(50, "How will you proceed?", FONT, LCD_BLACK, LCD_WHITE);
	  LCD_WriteStringCentered(100, "Confirm Move", FONT, LCD_BLACK, LCD_WHITE);
	  LCD_WriteStringCentered(150, "Return to Options", FONT, LCD_BLACK, LCD_WHITE); //maybe add conditional here for if combat is possible
	  LCD_FillRectangle(10, 50 + selection * y_pos, 10, 18, LCD_BLACK);

	  int backToMenu = 0;

	  key = '\0';
	  while (1) {
		  if (key == '#') {
			  key = '\0';
			  switch (selection) {
			  	  case (1):
					for (int hex = 0; hex < (int) possibleMoves.size(); hex++) {
						  int row = possibleMoves[hex]->GetHexRow();
						  int col = possibleMoves[hex]->GetHexColumn();
						  bool hallTrig = checkHallSensor(row, col, hmcps1, hmcps2);
						  if (hallTrig) {
							  //update buffer or map
							  mapBuffer[col + 16*row] = currHex->GetType();
							  character->SetColumn(col);
							  character->SetRow(row);
							  mapBuffer[currHex->GetHexColumn() + 16*currHex->GetHexRow()] = BaseHex;
							  movement = movement - map->HexDistance(currHex, possibleMoves[hex]);
							  map->GetHex(row,col)->SetType(currHex->GetType());
							  currHex->SetType(BaseHex);
							  currHex->SetPassable(true);
							  currHex = map->GetHex(row,col);
							  currHex->SetPassable(false);

							  uint8_t mapCharBuffer[256];
							  mapToBuffer(map, mapBuffer);
							  view = map->FieldOfView(map->GetHex(character->GetRow(), character->GetColumn()), character->GetVisibility());
							  FOVToBuffer(mapBuffer, view);
							  displayMap(htim1, htim3, mapBuffer, sizeof(mapBuffer) / sizeof(uint8_t));

								if (character->GetCharacterType() == Player) {
									view = map->FieldOfView(map->GetHex(character->GetRow(), character->GetColumn()), character->GetVisibility());
									for (int i = 0; i < view.size(); i++) {
										if (view[i]->GetType() == MonsterHex) {
											Character* monster = map->HexToCharacter(view[i]);
											if (monster->GetActive() == false) {
												LCD_FillScreen(LCD_WHITE);
												HAL_Delay(500);
												LCD_WriteStringCentered(50, "Monster Spotted!", FONT, LCD_BLACK, LCD_WHITE);
												HAL_Delay(2000);
												LCD_FillScreen(LCD_WHITE);
												HAL_Delay(500);
												LCD_WriteStringCentered(50, "Place token for", FONT, LCD_BLACK, LCD_WHITE);
												Character* monster = map->HexToCharacter(view[i]);
												std::pair<int, int> position = monster->GetPosition();
												monster->SetActive(true);
												std::string name = monster->GetName();
												const char* char_name = name.c_str();
												LCD_WriteStringCentered(70, char_name, FONT, LCD_BLACK, LCD_WHITE);
												std::memcpy(mapCharBuffer, mapBuffer, sizeof(uint8_t)* 256);

												while (1) {
													blinkLED(mapCharBuffer, position.first, position.second, MonsterHex);
													bool hallTrig = checkHallSensor(position.first, position.second, hmcps1, hmcps2);
													if (hallTrig) {
														mapBuffer[position.second + 16 * position.first] = MonsterHex;
														LCD_FillScreen(LCD_WHITE);
														HAL_Delay(500);
														break;
													}
												}
											}
										}
									}
								}

							 LCD_WriteStringCentered(50, "Move Piece", FONT, LCD_BLACK, LCD_WHITE);

							  y_pos = 50;
							  selection = 1;
							  prev_selection = 0;
							  //Give option for info, action, move
							  //LCD_WriteStringCentered(50, "How will you proceed?", FONT, LCD_BLACK, LCD_WHITE);
							  LCD_WriteStringCentered(100, "Confirm Move", FONT, LCD_BLACK, LCD_WHITE);
							  LCD_WriteStringCentered(150, "Return to Options", FONT, LCD_BLACK, LCD_WHITE); //maybe add conditional here for if combat is possible
							  LCD_FillRectangle(10, 50 + selection * y_pos, 10, 18, LCD_BLACK);

							  if (movement > 0) {
								  mapToBuffer(map, mapBuffer);
								  possibleMoves = map->PossibleMovements(currHex, movement);
								  mapHexesToBuffer(mapBuffer, possibleMoves, MoveHex);
								  view = map->FieldOfView(map->GetHex(character->GetRow(), character->GetColumn()), character->GetVisibility());
								  FOVToBuffer(mapBuffer, view);
								  displayMap(htim1, htim3, mapBuffer, sizeof(mapBuffer) / sizeof(uint8_t));

							  }
							  else {
								  mapToBuffer(map, mapBuffer);
								  view = map->FieldOfView(map->GetHex(character->GetRow(), character->GetColumn()), character->GetVisibility());
								  FOVToBuffer(mapBuffer, view);
								  displayMap(htim1, htim3, mapBuffer, sizeof(mapBuffer) / sizeof(uint8_t));
								  break;
							  }
						  }
					}

					  if (movement <= 0) {
						  mapToBuffer(map, mapBuffer);
						  view = map->FieldOfView(map->GetHex(character->GetRow(), character->GetColumn()), character->GetVisibility());
						  FOVToBuffer(mapBuffer, view);
						  displayMap(htim1, htim3, mapBuffer, sizeof(mapBuffer) / sizeof(uint8_t));
						  backToMenu = 1;
						  break;
					  }
					  break;
			  	  case (2):
				    mapToBuffer(map, mapBuffer);
				    view = map->FieldOfView(map->GetHex(character->GetRow(), character->GetColumn()), character->GetVisibility());
				    FOVToBuffer(mapBuffer, view);
					displayMap(htim1, htim3, mapBuffer, sizeof(mapBuffer) / sizeof(uint8_t));
					backToMenu = 1;
					break;
			  }
		  }
		  if(backToMenu){
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
			  LCD_FillRectangle(10, 50 + prev_selection * y_pos, 10, 18, LCD_WHITE);
			  LCD_FillRectangle(10, 50 + selection * y_pos, 10, 18, LCD_BLACK);
			  prev_selection = selection;
		  }

	  }
	  LCD_FillScreen(LCD_WHITE);
	  HAL_Delay(500);

	  bufferToMap(map, mapBuffer);
	  *_movement = movement;

	  return map;
}

void clearMap(TIM_HandleTypeDef htim1, TIM_HandleTypeDef htim3){
	for (int i= 0; i < 32; i++) {
		Set_LED(i, 0, 0, 0);
	}
	for (int ch = 1; ch < 5; ch++) {
	  Set_Brightness(20);
	  WS2812_Send(&htim1, ch);
	}
	for (int ch = 1; ch < 5; ch++) {
	  Set_Brightness(20);
	  WS2812_Send(&htim3, ch);
	}
}

GameMap* combatMode(TIM_HandleTypeDef htim1, TIM_HandleTypeDef htim3, MCP23017_HandleTypeDef hmcps1[8], MCP23017_HandleTypeDef hmcps2[8], GameMap* map, Hexagon* currHex, Character* character) {
	int targetType;
	if (character->GetCharacterType() == Player) {
		targetType = MonsterHex;
	}
	else {
		targetType = PlayerHex;
	}

	std::vector<Hexagon*> targetHex;
	if (character->GetClass() == Fighter) {
		std::vector<Hexagon*> neighbors = map->GetNeighbors(currHex);
		for (int hex = 0; hex < neighbors.size(); hex++) {
			if (neighbors[hex]->GetType() == targetType) {
				targetHex.push_back(neighbors[hex]);
			}
		}

		if (targetHex.size() == 0) {
			LCD_WriteStringCentered(50, "No Enemies in Range", FONT, LCD_BLACK, LCD_WHITE);
			HAL_Delay(2000);
			LCD_FillScreen(LCD_WHITE);
			HAL_Delay(500);

			return map;
		}
	}
	else if (character->GetClass() == Ranger) {
		std::vector<Hexagon*> fov = map->FieldOfView(currHex, character->GetVisibility());
		for (uint8_t i = 0; i < fov.size(); i++) {
			if (fov[i]->GetType() == targetType) {
				targetHex.push_back(fov[i]);
			}
		}

		if (targetHex.size() == 0) {
			LCD_WriteStringCentered(50, "No Enemies in Range", FONT, LCD_BLACK, LCD_WHITE);
			HAL_Delay(2000);
			LCD_FillScreen(LCD_WHITE);
			HAL_Delay(500);

			return map;
		}
	}


	int finalTargetIndex = 0;
	uint8_t mapBuffer[256];
	mapToBuffer(map, mapBuffer);
	std::vector<Hexagon*> view = map->FieldOfView(map->GetHex(character->GetRow(), character->GetColumn()), character->GetVisibility());
	FOVToBuffer(mapBuffer, view);

	uint8_t charMapBuffer[256];
	std::memcpy(charMapBuffer, mapBuffer, sizeof(uint8_t)* 256);

	int selection = 0;
	Character* targetSelection = NULL;
	for (int targetSelect = 0; targetSelect < characters->GetNumberCharacters(); targetSelect++) {
		if (characters->GetCharacter(targetSelect)->GetRow() == targetHex[selection]->GetHexRow() &&  characters->GetCharacter(targetSelect)->GetColumn() == targetHex[selection]->GetHexColumn()) {
			if (targetSelection->GetCharacterType() != DEAD) {
				targetSelection = characters->GetCharacter(targetSelect);
				finalTargetIndex = targetSelect;
				break;
			}
		}
	}

	LCD_WriteStringCentered(50, targetSelection->GetName().c_str(), FONT, LCD_BLACK, LCD_WHITE);
//	LCD_WriteStringCentered(50, ("Race: " + std::to_string(targetSelection->GetRace())).c_str(), FONT, LCD_BLACK, LCD_WHITE);
	LCD_WriteStringCentered(200, "Continue Attack", FONT, LCD_BLACK, LCD_WHITE);
	key = '\0';
	while (1) {
		blinkLED(charMapBuffer , targetHex[selection]->GetHexRow(), targetHex[selection]->GetHexColumn(), targetType);
		if (key == '#') {
			key = '\0';
			LCD_FillScreen(LCD_WHITE);
			HAL_Delay(500);
			int attackRoll = getRoll("Attack");
			if (attackRoll >= targetSelection->GetArmorClass() ) {
				LCD_FillScreen(LCD_WHITE);
				HAL_Delay(500);
				LCD_WriteStringCentered(50, "Attack Succeeded", FONT, LCD_BLACK, LCD_WHITE);
				HAL_Delay(2000);
				LCD_FillScreen(LCD_WHITE);
				HAL_Delay(500);
				int damageRoll = getRoll("Damage");
				targetSelection->SetCurrentHealthPoints(targetSelection->GetCurrentHealthPoints() - damageRoll);
				if ((character->GetClass() == Fighter) || (character->GetClass() == Ranger && map->HexDistance(currHex, targetHex[selection]) == 1)) {
				    attackMeleeHit(charMapBuffer, targetSelection->GetRow(), targetSelection->GetColumn(), targetSelection->GetCharacterType());
				}
				else if ((character->GetClass() == Ranger) && (map->HexDistance(currHex, targetHex[selection]) > 1)) {
					attackRangeHit(charMapBuffer, map->HexLineDraw(currHex, targetHex[selection]), targetSelection->GetCharacterType());
				}

				//if attack kills target
				if ((targetSelection->GetCurrentHealthPoints() - damageRoll) <= 0) {
					targetHex[selection]->SetType(BaseHex);
					targetHex[selection]->SetPassable(true);
					targetSelection->SetCharacterType(DEAD);

					mapBuffer[targetSelection->GetColumn() + 16*targetSelection->GetRow()] = BaseHex;
					view = map->FieldOfView(map->GetHex(character->GetRow(), character->GetColumn()), character->GetVisibility());
					FOVToBuffer(mapBuffer, view);
					displayMap(htim1, htim3, mapBuffer, sizeof(mapBuffer)/sizeof(uint8_t));

					LCD_FillScreen(LCD_WHITE);
					HAL_Delay(500);
					LCD_WriteStringCentered(30, (targetSelection->GetName() + " Killed").c_str(), FONT, LCD_BLACK, LCD_WHITE);
					HAL_Delay(2000);
				}
				else {
					//mapBuffer[targetSelection->GetColumn() + 16*targetSelection->GetRow()] = BaseHex;
					view = map->FieldOfView(map->GetHex(character->GetRow(), character->GetColumn()), character->GetVisibility());
					FOVToBuffer(mapBuffer, view);
					displayMap(htim1, htim3, mapBuffer, sizeof(mapBuffer)/sizeof(uint8_t));
				}

				break;
			}
			else {
				LCD_WriteStringCentered(50, "Attack Missed", FONT, LCD_BLACK, LCD_WHITE);
				HAL_Delay(2000);
				break;
			}
		}
		if (key == 'A') {
			key = '\0';
			int prevSelection = selection;
			selection = (selection > 0) ? selection - 1 : 0;
			//displayMap(htim1, htim3, mapBuffer, sizeof(mapBuffer)/sizeof(uint8_t));
			for (int targetSelect = 0; targetSelect < characters->GetNumberCharacters(); targetSelect++) {
				if (characters->GetCharacter(targetSelect)->GetRow() == targetHex[selection]->GetHexRow() &&  characters->GetCharacter(targetSelect)->GetColumn() == targetHex[selection]->GetHexColumn()) {
					targetSelection = characters->GetCharacter(targetSelect);
				}
			}

			if (selection != prevSelection) {
				LCD_FillScreen(LCD_WHITE);
//				HAL_Delay(500);
				LCD_WriteStringCentered(50, targetSelection->GetName().c_str(), FONT, LCD_BLACK, LCD_WHITE);
	//			LCD_WriteStringCentered(50, ("Race: " + std::to_string(targetSelection->GetRace())).c_str(), FONT, LCD_BLACK, LCD_WHITE);

				//LCD_WriteStringCentered(70, ("Class: " + std::to_string(targetSelection->GetClass())).c_str(), FONT, LCD_BLACK, LCD_WHITE);
				LCD_WriteStringCentered(200, "Continue Attack", FONT, LCD_BLACK, LCD_WHITE);
			}
		}
		if (key == 'D') {
			key = '\0';
			int prevSelection = selection;
			selection = (selection < (targetHex.size()-1)) ? selection + 1 : (targetHex.size() - 1);
			//displayMap(htim1, htim3, mapBuffer, sizeof(mapBuffer)/sizeof(uint8_t));
			for (int targetSelect = 0; targetSelect < characters->GetNumberCharacters(); targetSelect++){
				if(characters->GetCharacter(targetSelect)->GetRow() == targetHex[selection]->GetHexRow() && characters->GetCharacter(targetSelect)->GetColumn() == targetHex[selection]->GetHexColumn()) {
					targetSelection = characters->GetCharacter(targetSelect);
				}
			}

			if (selection != prevSelection) {
//				LCD_FillScreen(LCD_WHITE);
//				HAL_Delay(500);
				LCD_WriteStringCentered(50, targetSelection->GetName().c_str(), FONT, LCD_BLACK, LCD_WHITE);
	//			LCD_WriteStringCentered(50, ("Race: " + std::to_string(targetSelection->GetRace())).c_str(), FONT, LCD_BLACK, LCD_WHITE);

				//LCD_WriteStringCentered(70, ("Class: " + std::to_string(targetSelection->GetClass())).c_str(), FONT, LCD_BLACK, LCD_WHITE);
				LCD_WriteStringCentered(200, "Continue Attack", FONT, LCD_BLACK, LCD_WHITE);
			}
		}
		if (key == '*') {
		  key = '\0';
		  break;
		}
	}

	LCD_FillScreen(LCD_WHITE);
	HAL_Delay(500);
	//mapBuffer[targetSelection->GetColumn() + 16*targetSelection->GetRow()] = BaseHex;
	view = map->FieldOfView(map->GetHex(character->GetRow(), character->GetColumn()), character->GetVisibility());
	FOVToBuffer(mapBuffer, view);
	displayMap(htim1, htim3, mapBuffer, sizeof(mapBuffer) / sizeof(uint8_t));
	bufferToMap(map, mapBuffer);

	return map;
}


void blinkLED(uint8_t* mapCharBuffer , int row, int col, int type) {
	HAL_Delay(500);
	mapCharBuffer[col + 16 * row] = 0;
	displayMap(htim1, htim3, mapCharBuffer, 256);
	mapCharBuffer[col + 16 * row] = type;
	HAL_Delay(500);
	displayMap(htim1, htim3, mapCharBuffer, 256);
}

void FOVToBuffer(uint8_t* mapBuffer, std::vector<Hexagon*> hexes){
	uint8_t fovBuffer[256] = {0};
	int sizeTest2 = hexes.size();
	for (uint8_t i = 0; i < hexes.size(); i++) {
		 int colN = hexes[i]->GetHexColumn();
		 int rowN = hexes[i]->GetHexRow();
		 if(colN >= 16 || rowN >= 16){
			 sizeTest2 = 0;
		 }

		 fovBuffer[colN + 16*rowN] = 1;
		// hexes[i]->GetType();
		// mapBuffer[colN + 16*rowN] = 1;
	}
	int sizeTest = sizeof(fovBuffer);
	for (int i = 0; i < 256; i++) {
		if (fovBuffer[i] == 0) {
			mapBuffer[i] = 0;
		}
	}
}

int getRoll(const std::string& inputStr) {
	LCD_WriteStringCentered(50, ("Insert " + inputStr).c_str(), FONT, LCD_BLACK, LCD_WHITE);
	LCD_WriteStringCentered(70, "Roll", FONT, LCD_BLACK, LCD_WHITE);
	char* roll_string = new char[3];
	int no_character = 0;
	int start_tick = HAL_GetTick();
	key = '\0';
	while (1) {
		int cur_tick = HAL_GetTick();
		if ((cur_tick - start_tick) >= 60000) {
			LCD_FillScreen(LCD_WHITE);
			HAL_Delay(500);
			return 0;
		}

		if (key == '#' && no_character != 0) {
			key = '\0';
			roll_string[no_character] = '\0';
			LCD_FillScreen(LCD_WHITE);
			HAL_Delay(500);
			break;
		}
		else if (key == '*') {
			key = '\0';
			if (no_character > 0) {
				LCD_WriteStringCentered(110, roll_string, FONT, LCD_WHITE, LCD_WHITE);
				no_character--;
				roll_string[no_character] = '\0';
				LCD_WriteStringCentered(110, roll_string, FONT, LCD_BLACK, LCD_WHITE);
			}
		}
		else if (no_character < 2 && Key_Is_Number(key)) {
			LCD_WriteStringCentered(110, roll_string, FONT, LCD_WHITE, LCD_WHITE);
			roll_string[no_character] = key;
			no_character++;
			roll_string[no_character] = '\0';
			LCD_WriteStringCentered(110, roll_string, FONT, LCD_BLACK, LCD_WHITE);
			key = '\0';
		}
	}

	return atoi(roll_string);
}

void attackMeleeHit(uint8_t* mapCharBuffer, int row, int col, int type) {
	for (int i = 0; i < 3; i++) {
		if (type == Player){
			mapCharBuffer[col + 16 * row] = 6;
		}
		else {
			mapCharBuffer[col + 16 * row] = 7;
		}
		displayMap(htim1, htim3, mapCharBuffer, 256);
		HAL_Delay(50);
		mapCharBuffer[col + 16 * row] = type;
		displayMap(htim1, htim3, mapCharBuffer, 256);
		HAL_Delay(100);
	}
}

void attackRangeHit(uint8_t* mapCharBuffer, std::vector<Hexagon*> line, int type) {
	uint8_t original_buffer[256];
	std::memcpy(original_buffer, mapCharBuffer, sizeof(uint8_t) * 256);
	uint8_t i = 0;
	for (i = 1; i < line.size()-1; i++) {
		Hexagon* hex = line[i];
		int row = hex->GetHexRow();
		int col = hex->GetHexColumn();
		if (type == Player) {
			mapCharBuffer[col + 16 * row] = 6;
		}
		else {
			mapCharBuffer[col + 16 * row] = 7;
		}
		displayMap(htim1, htim3, mapCharBuffer, 256);
		HAL_Delay(50);
	}
	attackMeleeHit(mapCharBuffer, line[i]->GetHexRow(), line[i]->GetHexColumn(), type);
}

GameMap* chestMode(TIM_HandleTypeDef htim1, TIM_HandleTypeDef htim3, MCP23017_HandleTypeDef hmcps1[8], MCP23017_HandleTypeDef hmcps2[8], GameMap* map, Hexagon* currHex, Character* character) {

	std::vector<Hexagon*> neighbors = map->GetNeighbors(currHex);
	std::vector<Hexagon*> lootHex;
	int minGold = 10;
	int maxGold = 100;
	int goldVal = 0;
    int start = HAL_GetTick();

    // Convert the current time to milliseconds since the epoch

	for (uint8_t hex = 0; hex < neighbors.size(); hex++) {
		if (neighbors[hex]->GetType() == ChestHex ) {
			lootHex.push_back(neighbors[hex]);
		}
	}

	if (lootHex.size() == 0) {
		LCD_WriteStringCentered(50, "No Chests in Range", FONT, LCD_BLACK, LCD_WHITE);
		HAL_Delay(2000);
		LCD_FillScreen(LCD_WHITE);
		HAL_Delay(500);

		return map;
	}

	uint8_t mapBuffer[256];
	mapToBuffer(map, mapBuffer);
	std::vector<Hexagon*> view = map->FieldOfView(map->GetHex(character->GetRow(), character->GetColumn()), character->GetVisibility());
	FOVToBuffer(mapBuffer, view);

	uint8_t charMapBuffer[256];
	std::memcpy(charMapBuffer, mapBuffer, sizeof(uint8_t)* 256);

	int selection = 0;

	LCD_WriteStringCentered(50, "Open Chest", FONT, LCD_BLACK, LCD_WHITE);
	LCD_WriteStringCentered(200, "Continue", FONT, LCD_BLACK, LCD_WHITE);
	LCD_FillRectangle(10, 200, 10, 18, LCD_BLACK);
	key = '\0';
	while (1) {
		blinkLED(charMapBuffer , lootHex[selection]->GetHexRow(), lootHex[selection]->GetHexColumn(), ChestHex);
		if (key == '#') {
			key = '\0';

			openChest(charMapBuffer, lootHex[selection]->GetHexRow(), lootHex[selection]->GetHexColumn());
			lootHex[selection]->SetType(BaseHex);
			lootHex[selection]->SetPassable(true);
			int end = HAL_GetTick();
		    goldVal = (end - start) % 100 + 1;

			mapBuffer[lootHex[selection]->GetHexColumn() + 16*lootHex[selection]->GetHexRow()] = BaseHex;
			view = map->FieldOfView(map->GetHex(character->GetRow(), character->GetColumn()), character->GetVisibility());
			FOVToBuffer(mapBuffer, view);
			displayMap(htim1, htim3, mapBuffer, sizeof(mapBuffer)/sizeof(uint8_t));


			LCD_FillScreen(LCD_WHITE);
			HAL_Delay(500);
			LCD_WriteStringCentered(50, (std::to_string(goldVal) + " Gold Received!").c_str(), FONT, LCD_BLACK, LCD_WHITE);
			character->SetGold(character->GetGold() + goldVal);
			HAL_Delay(2000);
			LCD_WriteStringCentered(100, (character->GetName() + " Has " + std::to_string(character->GetGold()) + " Gold" ).c_str(), FONT, LCD_BLACK, LCD_WHITE);
			HAL_Delay(2000);
			break;

		}
		if (key == 'A') {
			key = '\0';
			int prevSelection = selection;
			selection = (selection > 0) ? selection - 1 : 0;
			LCD_WriteStringCentered(50, "Open Chest", FONT, LCD_BLACK, LCD_WHITE);

		}
		if (key == 'D') {
			key = '\0';
			int prevSelection = selection;
			selection = (selection < (lootHex.size()-1)) ? selection + 1 : (lootHex.size() - 1);

			LCD_WriteStringCentered(50, "Open Chest", FONT, LCD_BLACK, LCD_WHITE);
		}
		if (key == '*') {
		  key = '\0';
		  break;
		}
	}

	LCD_FillScreen(LCD_WHITE);
	HAL_Delay(500);
	//mapBuffer[targetSelection->GetColumn() + 16*targetSelection->GetRow()] = BaseHex;
	view = map->FieldOfView(map->GetHex(character->GetRow(), character->GetColumn()), character->GetVisibility());
	FOVToBuffer(mapBuffer, view);
	displayMap(htim1, htim3, mapBuffer, sizeof(mapBuffer) / sizeof(uint8_t));
	bufferToMap(map, mapBuffer);

	return map;
}


void openChest(uint8_t* mapCharBuffer , int row, int col){
	for (int i = 0; i < 3; i++) {
		mapCharBuffer[col + 16 * row] = ChestHex;
		displayMap(htim1, htim3, mapCharBuffer, 256);
		HAL_Delay(100);
		mapCharBuffer[col + 16 * row] = BaseHex;
		displayMap(htim1, htim3, mapCharBuffer, 256);
		HAL_Delay(100);
	}
}

