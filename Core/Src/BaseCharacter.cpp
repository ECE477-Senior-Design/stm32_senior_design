#include "BaseCharacter.h"

//Constructor for character class that keeps track of hex position, ability score, and some combat values
Character::Character(std::string name, int column, int row,
        int strength, int dexterity, int constitution, int intelligence, int wisdom, int charisma,
        int max_health_points, int current_health_points, int armor_class, int initiative, int speed, int gold, int visibility) {
    _name = name;
    _column = column;
    _row = row;
    _strength = strength;
    _dexterity = dexterity;
    _constitution = constitution;
    _intelligence = intelligence;
    _wisdom = wisdom;
    _charisma = charisma;
    _max_health_points = max_health_points;
    _current_health_points = current_health_points;
    _armor_class = armor_class;
    _initiative = initiative;
    _speed = speed;
    _gold = gold;
    _visibility = visibility;
}

//Returns name of character
std::string Character::GetName(void) {
    return _name;
}

//Returns column position of character
int Character::GetColumn(void) {
    return _column;
}

//Returns row position of character
int Character::GetRow(void) {
    return _row;
}

std::pair<int, int> Character::GetPosition(void) {
    return std::make_pair(_row, _column);
}

//Returns strength of character
int Character::GetStrength(void) {
    return _strength;
}

//Returns dexterity of character
int Character::GetDexterity(void) {
    return _dexterity;
}

//Returns constitution of character
int Character::GetConstitution(void) {
    return _constitution;
}

//Returns intelligence of character
int Character::GetIntelligence(void) {
    return _intelligence;
}

//Returns wisdom of character
int Character::GetWisdom(void) {
    return _wisdom;
}

//Returns charisma of character
int Character::GetCharisma(void) {
    return _charisma;
}

//Returns max health points of character
int Character::GetMaxHealthPoints(void) {
    return _max_health_points;
}

//Returns current health points of character
int Character::GetCurrentHealthPoints(void) {
    return _current_health_points;
}

//Returns armor class of character
int Character::GetArmorClass(void) {
    return _armor_class;
}

//Returns initiative of character
int Character::GetInitiative(void) const {
    return _initiative;
}

//Returns speed of character
int Character::GetSpeed(void) {
    return _speed;
}

int Character::GetGold(void) {
	return _gold;
}

int Character::GetVisibility(void) {
	return _visibility;
}

//Displays character info
void Character::DisplayCharacterInfo(void) {
    std::cout << "Name: " << _name << std::endl;
    std::cout << "Column: " << _column << std::endl;
    std::cout << "Row: " << _row << std::endl;
    std::cout << "Strength: " << _strength << std::endl;
    std::cout << "Dexterity: " << _dexterity << std::endl;
    std::cout << "Constitution: " << _constitution << std::endl;
    std::cout << "Intelligence: " << _intelligence << std::endl;
    std::cout << "Wisdom: " << _wisdom << std::endl;
    std::cout << "Charisma: " << _charisma << std::endl;
    std::cout << "Max Health Points: " << _max_health_points << std::endl;
    std::cout << "Current Health Points: " << _current_health_points << std::endl;
    std::cout << "Armor Class: " << _armor_class << std::endl;
    std::cout << "Initiative: " << _initiative << std::endl;
    std::cout << "Speed: " << _speed << std::endl;
    std::cout << "Gold: " << _gold << std:: endl;
    std::cout << "Visibility" << _visibility << std::endl;
}

//Returns type of character
CharacterType Character::GetCharacterType(void) {
    return _character_type;
}

//Returns class of character
Class Character::GetClass(void) {
    return _class;
}

bool Character::GetActive(void){
	return _active;
}

//Sets column position of character
void Character::SetColumn(int column) {
    _column = column;
}

//Sets row position of character
void Character::SetRow(int row) {
    _row = row;
}

//Sets strength of character
void Character::SetStrength(int strength) {
    _strength = strength;
}

//Sets dexterity of character
void Character::SetDexterity(int dexterity) {
    _dexterity = dexterity;
}

//Sets constitution of character
void Character::SetConstitution(int constitution) {
    _constitution = constitution;
}

//Sets intelligence of character
void Character::SetIntelligence(int intelligence) {
    _intelligence = intelligence;
}

//Sets wisdom of character
void Character::SetWisdom(int wisdom) {
    _wisdom = wisdom;
}

//Sets charisma of character
void Character::SetCharisma(int charisma) {
    _charisma = charisma;
}

//Sets max health points of character
void Character::SetMaxHealthPoints(int max_health_points) {
    _max_health_points = _max_health_points;
}

//Sets current hit points of character
void Character::SetCurrentHealthPoints(int current_health_points) {
    _current_health_points = current_health_points;
}

//Sets armor class of character
void Character::SetArmorClass(int armor_class) {
    _armor_class = armor_class;
}

//Sets initiative of character
void Character::SetInitiative(int initiative) {
    _initiative = initiative;
}

//Sets speed of character
void Character::SetSpeed(int speed) {
    _speed = speed;
}

void Character::SetGold(int gold) {
	_gold = gold;
}

void Character::SetVisibility(int visibility) {
	_visibility = visibility;
}

//Sets type of character
void Character::SetCharacterType(CharacterType character_type) {
    _character_type = character_type;
}

//Sets class of character
void Character::SetClass(Class class_) {
    _class = class_;
}

void Character::SetActive(bool active){
	_active = active;
}

//Returns modifier based on the ability score
int Character::CalculateModifier(int ability_score) {
    int modifier;
    switch(ability_score) {
        case 1: modifier = -5; break;
        case 2: modifier = -4; break;
        case 3: modifier = -4; break;
        case 4: modifier = -3; break;
        case 5: modifier = -3; break;
        case 6: modifier = -2; break;
        case 7: modifier = -2; break;
        case 8: modifier = -1; break;
        case 9: modifier = -1; break;
        case 10: modifier = 0; break;
        case 11: modifier = 0; break;
        case 12: modifier = 1; break;
        case 13: modifier = 1; break;
        case 14: modifier = 2; break;
        case 15: modifier = 2; break;
        case 16: modifier = 3; break;
        case 17: modifier = 3; break;
        case 18: modifier = 4; break;
        case 19: modifier = 4; break;
        case 20: modifier = 5; break;
        case 21: modifier = 5; break;
        case 22: modifier = 6; break;
        case 23: modifier = 6; break;
        case 24: modifier = 7; break;
        case 25: modifier = 7; break;
        case 26: modifier = 8; break;
        case 27: modifier = 8; break;
        case 28: modifier = 9; break;
        case 29: modifier = 9; break;
        case 30: modifier = 10; break;
    }

    return modifier;
}

//Destructor for character class
Character::~Character() {
}
