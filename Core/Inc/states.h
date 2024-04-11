/*
 * states.h
 *
 *  Created on: Apr 4, 2024
 *      Author: neilt
 */

#ifndef INC_STATES_H_
#define INC_STATES_H_

#include "GameMap.h"
#include "GameCharacters.h"
#include "main.h"
#include "lcd.h"
#include "keypad.h"
#include "usb.h"
#include "boardLighting.h"
#include "ws2812b.h"
#include "displayFuncs.h"

typedef enum {
	WELCOME_STATE,
	MENU_STATE,
	DM_MODE_STATE,
	PLAYING_MODE_STATE,
	UPLOAD_MAP_STATE,
	VIEW_MAP_STATE,
	GAME_START_STATE
} GameState;

void LED_Test(void);
void Welcome(void);
void Menu(void);
void DM_Mode(void);
void Upload_Map(void);
void View_Map(void);
void Playing_Mode(void);
void Game_Start(void);

#endif /* INC_STATES_H_ */
