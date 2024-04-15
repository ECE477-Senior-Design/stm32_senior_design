/*
MOST RECENT AUTHOR: JACKSON
DATE: 10/31/2023
*/

#ifndef MONSTERFIGHTER_H
#define MONSTERFIGHTER_H

#include "BaseCharacter.h"

//Class definition of a fighter class for monster, derived from character class
class MonsterFighter : public Character {
    public:
        MonsterFighter(int column, int row,
        int strength, int dexterity, int constitution, int intelligence, int wisdom, int charisma,
        int max_health_points, int current_health_points, int armor_class, int initiative, int speed, int gold, int visibility); //Constructor for fighter class for monster
};

#endif
