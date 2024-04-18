/*
MOST RECENT AUTHOR: JACKSON
DATE: 10/31/2023
*/

#include "MonsterFighter.h"

//Constructor for fighter class for monster
MonsterFighter::MonsterFighter(int column, int row,
        int strength, int dexterity, int constitution, int intelligence, int wisdom, int charisma,
        int max_health_points, int current_health_points, int armor_class, int initiative, int speed, int gold, int visibility) :
        Character(NULL, column, row,
        strength, dexterity, constitution, intelligence, wisdom, charisma,
        max_health_points, current_health_points, armor_class, initiative, speed, gold, visibility) {
    _character_type = Monster; //Sets character type to monster
    _class = Fighter; //Sets class to fighter
    _active = false;
}
