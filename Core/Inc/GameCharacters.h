/*
MOST RECENT AUTHOR: JACKSON
DATE: 10/27/2023
*/

#ifndef GAMECHARACTERS_H
#define GAMECHARACTERS_H

#include <vector>
#include <algorithm>
#include "BaseCharacter.h"

//Class for both player and monster characters
class GameCharacters {
    protected:
        int _number_characters; //Number of characters in characters vector
        std::vector<Character*> charactersVec; //Characters vector

    public:
        GameCharacters(int number_characters); //Constructor of characters vector
        GameCharacters(std::vector<std::string> input_strlist); //Constructor of characters vector from string

        int GetNumberCharacters(void); //Returns number of characters
        int GetNumberPlayers(void);
        int GetNumberMonsters(void);
//        void AddCharacter(int index, std::string name, int column, int row,
//        int strength, int dexterity, int constitution, int intelligence, int wisdom, int charisma,
//        int max_health_points, int current_health_points, int armor_class, int initiative, int speed, CharacterType character_type, Class class_); //Adds character to character vector
        std::vector<Character*> GetCharactersVec(void);
        Character* GetCharacter(int index); //Returns character at index
        ~GameCharacters(); //Destructor for characters vector


        void SortCharacters(void) {
        	std::sort(charactersVec.begin(), charactersVec.end(), [](Character* character1, Character* character2) { //removed const from bother characters
        		return character1->GetInitiative() > character2->GetInitiative();
        	});
        }

        void setNumChar(int number_characters);

};

#endif
