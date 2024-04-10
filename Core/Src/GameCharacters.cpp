/*
MOST RECENT AUTHOR: JACKSON
DATE: 10/27/2023
*/


#include "GameCharacters.h"

//Constructor of characters vector
//GameCharacters::GameCharacters(int number_characters) : _number_characters(number_characters) {
//
//}

//Constructor of characters vector from string
GameCharacters::GameCharacters(std::vector<std::string> input_strlist) {
    _number_characters = input_strlist.size(); //original input string list contains number of characters -1 for the map
    std::string delimiter = ",";
    std::vector<std::string> character_info;
    for(size_t i = 0; i < input_strlist.size(); i++) {
        character_info.clear();
        std::string input = input_strlist[i];
        size_t pos = 0;
        std::string token;
        while ((pos = input.find(delimiter)) != std::string::npos) {
            token = input.substr(0, pos);
            character_info.push_back(token);
            input.erase(0, pos + delimiter.length());
        }
        character_info.push_back(input);

        Character character = Character(character_info[0], std::stoi(character_info[1]), std::stoi(character_info[2]),
            std::stoi(character_info[3]), std::stoi(character_info[4]), std::stoi(character_info[5]), std::stoi(character_info[6]),
            std::stoi(character_info[7]), std::stoi(character_info[8]), std::stoi(character_info[9]), std::stoi(character_info[10]),
            std::stoi(character_info[11]), std::stoi(character_info[12]), std::stoi(character_info[13]));

        character.SetCharacterType(static_cast<CharacterType> (std::stoi(character_info[14])));
        character.SetClass(static_cast<Class> (std::stoi(character_info[15])));

        characters.push_back(character);
    }

}

//Returns number of characters
int GameCharacters::GetNumberCharacters(void) {
    return _number_characters;
}

//Adds character to character vector
void GameCharacters::AddCharacter(int index, std::string name, int column, int row,
        int strength, int dexterity, int constitution, int intelligence, int wisdom, int charisma,
        int max_health_points, int current_health_points, int armor_class, int initiative, int speed, CharacterType character_type, Class class_) {
    Character character = Character(name, column, row,
        strength, dexterity, constitution, intelligence, wisdom, charisma,
        max_health_points, current_health_points, armor_class, initiative, speed);
    character.SetClass(class_); //Sets class of character
    character.SetCharacterType(character_type); //Sets character type
    characters[index] = character; //Adds character to characters vector
}

//Returns character at index
Character& GameCharacters::GetCharacter(int index) {
    return characters.at(index);
}

//Destructor for characters vector
GameCharacters::~GameCharacters() {
    //Deletes each character in characters vector
    for (int i = 0; i < _number_characters; i++) {
        if (&characters[i] != nullptr) {
            delete &characters[i];
        }
    }
}
