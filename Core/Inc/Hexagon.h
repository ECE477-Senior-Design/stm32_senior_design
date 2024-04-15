/*
MOST RECENT AUTHOR: JACKSON
DATE: 10/31/2023
*/

#ifndef HEXAGON_HH
#define HEXAGON_HH

#include <climits>
#include <vector>
#include <queue>
#include <unordered_set>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <unordered_map>
#include <set>

//Defines the type of hexes (eventually add chest)
enum HexagonType {
    BaseHex = 8,
    WallHex = 1,
    PlayerHex = 2,
    MonsterHex = 3,
	ChestHex = 4,
	MoveHex = 5,
//	PlayerHexTurn,
};

// //Includes weight for each hex (everything except for base should be high)
// enum Weight {
//     Passable = 1,
//     Impassable = 10000,
// };


#endif
