#include "displayFuncs.h"

extern GameState game_state;
extern char key;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;

extern GameMap* map;
extern GameCharacters * characters;
extern MCP23017_HandleTypeDef hmcps1[8];
extern MCP23017_HandleTypeDef hmcps2[8];

//helper function for View_Character_Info
void Display_Stats(Character *c, int select){

	LCD_FillScreen(LCD_WHITE);
	if(select == 0){
		LCD_WriteStringCentered(50, c->GetName().c_str(), FONT, LCD_BLACK, LCD_WHITE);

		std::string concatenated = "HP: " + std::to_string(c->GetCurrentHealthPoints()) + "/" + std::to_string(c->GetMaxHealthPoints());
		LCD_WriteString(30, 80, concatenated.c_str(), FONT, LCD_BLACK, LCD_WHITE);
		LCD_WriteString(30, 100, ("Armor Class: " + std::to_string(c->GetArmorClass())).c_str(), FONT, LCD_BLACK, LCD_WHITE);
		LCD_WriteString(30, 120, ("Init: " + std::to_string(c->GetInitiative())).c_str(), FONT, LCD_BLACK, LCD_WHITE);
		LCD_WriteString(30, 140, ("Speed: " + std::to_string(c->GetIntelligence())).c_str(), FONT, LCD_BLACK, LCD_WHITE);

		LCD_WriteStringCentered(240, "View Attributes", FONT, LCD_BLACK, LCD_WHITE);
		LCD_WriteStringCentered(260, "Return", FONT, LCD_BLACK, LCD_WHITE);
	}
	else if(select == 1){
		LCD_WriteStringCentered(50, c->GetName().c_str(), FONT, LCD_BLACK, LCD_WHITE);
		LCD_WriteString(30, 80, ("Str: " + std::to_string(c->GetStrength())).c_str(), FONT, LCD_BLACK, LCD_WHITE);
		LCD_WriteString(30, 100, ("Dex: " + std::to_string(c->GetDexterity())).c_str(), FONT, LCD_BLACK, LCD_WHITE);
		LCD_WriteString(30, 120, ("Con: " + std::to_string(c->GetConstitution())).c_str(), FONT, LCD_BLACK, LCD_WHITE);
		LCD_WriteString(30, 140, ("Int: " + std::to_string(c->GetIntelligence())).c_str(), FONT, LCD_BLACK, LCD_WHITE);
		LCD_WriteString(30, 160, ("Wis: " + std::to_string(c->GetWisdom())).c_str(), FONT, LCD_BLACK, LCD_WHITE);
		LCD_WriteString(30, 180, ("Cha: " + std::to_string(c->GetCharisma())).c_str(), FONT, LCD_BLACK, LCD_WHITE);

		LCD_WriteStringCentered(240, "View Stats", FONT, LCD_BLACK, LCD_WHITE);
		LCD_WriteStringCentered(260, "Return", FONT, LCD_BLACK, LCD_WHITE);
	}

}

void View_Character_Info(Character * c){
	int selection = 1;
	int prev_selection = 0;
	int y_pos = 20;
	int page = 0;

	LCD_FillScreen(LCD_WHITE);
	Display_Stats(c, page);
	LCD_FillRectangle(10, selection * y_pos + 220, 10, 18, LCD_BLACK);

	while (1) {
		if (key == '#') {
			key = '\0';
			switch (selection) {
				case (1):
					if(page == 0){
						page = 1;
						Display_Stats(c, page);
						LCD_FillRectangle(10, selection * y_pos + 220, 10, 18, LCD_BLACK);
						key = '\0';
					}
					else if(page == 1){
						page = 0;
						Display_Stats(c, page);
						LCD_FillRectangle(10, selection * y_pos + 220, 10, 18, LCD_BLACK);
						key = '\0';
					}
					continue;
				case (2):
					LCD_FillScreen(LCD_WHITE);
					HAL_Delay(500);
					return;
			}
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
			LCD_FillRectangle(10, prev_selection * y_pos + 220, 10, 18, LCD_WHITE);
			LCD_FillRectangle(10, selection * y_pos + 220, 10, 18, LCD_BLACK);
			prev_selection = selection;
		}
	}
}
