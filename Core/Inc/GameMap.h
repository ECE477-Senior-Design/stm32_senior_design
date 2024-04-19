/*
MOST RECENT AUTHOR: JACKSON
DATE: 10/27/2023
*/

#ifndef GAMEMAP_HH
#define GAMEMAP_HH

#include "BaseHexagon.h"
#include "GameCharacters.h"

//Node structure to keep track of column, row, and distance of each hex for shortest path
struct AStarNode {
    Hexagon* hex;
    AStarNode* parent;
    int g_cost;
    int h_cost;

    AStarNode(Hexagon* hexagon, AStarNode* parent_node, int g, int h) : hex(hexagon), parent(parent_node), g_cost(g), h_cost(h) {}

    int GetFCost() const { return g_cost + h_cost; }
};

class GameMap {
    private:
        int _rows;
        int _columns;
        std::vector<std::vector<Hexagon*>> map;

    public:
        GameMap(int rows, int columns);
        GameMap(std::string mapstring);

        int GetRows(void); //Returns the number of rows
        int GetColumns(void); //Returns the number of columns

        Hexagon* GetHex(int row, int column);
        void ChangeHex(int row, int column, HexagonType type);
        Character* HexToCharacter(Hexagon* hexagon);

        std::vector<Hexagon*> GetNeighbors(Hexagon* hexagon);
        std::vector<Hexagon*> PossibleMovements(Hexagon* start, int movement);
        int HexDistance(Hexagon* start, Hexagon* end);
		double Lerp(double a, double b, double t);
		std::pair<double, double> HexLerp(Hexagon* a, Hexagon* b, double t);
		std::vector<double> AxialCube(std::pair<double, double> coordinates);
		Hexagon* HexRound(std::vector<double> coordinates);
		std::vector<Hexagon*> HexLineDraw(Hexagon* start, Hexagon* end);
		std::vector<Hexagon*> FieldOfView(Hexagon* start, int range);
        std::vector<Hexagon*> FindClosestPlayer(Hexagon* monster_hexagon, std::vector<Hexagon*> characters_hexagons);
        std::vector<Hexagon*> PathFind(Hexagon* start, Hexagon* end);

        void PrintMap(void);

        ~GameMap();
};

#endif
