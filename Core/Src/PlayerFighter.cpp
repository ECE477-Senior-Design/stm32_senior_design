/*
MOST RECENT AUTHOR: JACKSON
DATE: 10/31/2023
*/

#include "PlayerFighter.h"

//Constructor for fighter class for player
PlayerFighter::PlayerFighter(std::string name, int column, int row,
        int strength, int dexterity, int constitution, int intelligence, int wisdom, int charisma,
        int max_health_points, int current_health_points, int armor_class, int initiative, int speed, int gold, int visibility) :
        Character(name, column, row,
        strength, dexterity, constitution, intelligence, wisdom, charisma,
        max_health_points, current_health_points, armor_class, initiative, speed, gold, visibility) {
    _character_type = Player; //Sets character type to player
    _class = Fighter; //Sets class to fighter
}
