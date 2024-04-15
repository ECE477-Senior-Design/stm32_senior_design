/*
MOST RECENT AUTHOR: JACKSON
DATE: 10/31/2023
*/

#ifndef PLAYERFIGHTER_H
#define PLAYERFIGHTER_H

#include "BaseCharacter.h"

//Class definition of a fighter class for player, derived from character class
class PlayerFighter : public Character {
    public:
        PlayerFighter(std::string name, int column, int row,
        int strength, int dexterity, int constitution, int intelligence, int wisdom, int charisma,
        int max_health_points, int current_health_points, int armor_class, int initiative, int speed, int gold, int visibility); //Constructor for fighter class for player
};

#endif
