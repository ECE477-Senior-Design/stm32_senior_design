#include "GameCharacters.h"

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

        Character* character = new Character(character_info[0], std::stoi(character_info[1]), std::stoi(character_info[2]),
            std::stoi(character_info[3]), std::stoi(character_info[4]), std::stoi(character_info[5]), std::stoi(character_info[6]),
            std::stoi(character_info[7]), std::stoi(character_info[8]), std::stoi(character_info[9]), std::stoi(character_info[10]),
            std::stoi(character_info[11]), std::stoi(character_info[12]), std::stoi(character_info[13]), std::stoi(character_info[14]),
			std::stoi(character_info[15]));

        character->SetCharacterType(static_cast<CharacterType> (std::stoi(character_info[17])));
        character->SetClass(static_cast<Class> (std::stoi(character_info[18])));
        if (character->GetCharacterType() == Monster) {
        	character->SetActive(false);
        }

        charactersVec.push_back(character);
    }

}

//Returns number of characters
int GameCharacters::GetNumberCharacters(void) {
    return _number_characters;
}

int GameCharacters::GetNumberPlayers(void) {
	int num = 0;
	for(int i = 0; i < _number_characters; i++){
		if(charactersVec[i]->GetCharacterType() == Player){
			num++;
		}
	}
	return num;
}

int GameCharacters::GetNumberMonsters(void) {
	int num = 0;
	for(int i = 0; i < _number_characters; i++){
		if(charactersVec[i]->GetCharacterType() == Monster){
			num++;
		}
	}
	return num;
}

//Returns character at index
Character* GameCharacters::GetCharacter(int index) {
    return charactersVec[index];
}

//Destructor for characters vector
GameCharacters::~GameCharacters() {
    //Deletes each character in characters vector
    for (int i = 0; i < _number_characters; i++) {
        if (&charactersVec[i] != nullptr) {
            delete &charactersVec[i];
        }
    }
}

std::vector<Character*> GameCharacters::GetCharactersVec(){
	return charactersVec;
}
