#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <vector>
#include <iostream>

enum CharacterType {
    Player,
    Monster,
	DEAD,
};

//Defines the class of the character (eventually add ranger and wizard)
enum Class {
    Fighter,
	Ranger
};

#endif
