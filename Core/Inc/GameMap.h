/*
MOST RECENT AUTHOR: JACKSON
DATE: 10/27/2023
*/

#ifndef GAMEMAP_HH
#define GAMEMAP_HH

#include "BaseHexagon.h"

//Node structure to keep track of column, row, and distance of each hex for shortest path
struct AStarNode {
    Hexagon* hex;
    AStarNode* parent;
    int g_cost;
    int h_cost;

    AStarNode(Hexagon* hexagon, AStarNode* parent_node, int g, int h) : hex(hexagon), parent(parent_node), g_cost(g), h_cost(h) {}

    int GetFCost() const { return g_cost + h_cost; }
};

// //Class definiton for the game map and its associated functions
// class GameMap {
//     private:
//         int _columns; //Number of columns in map (x-coordinate)
//         int _rows; //Number of rows in map (y-coordinate)
//         std::vector<std::vector<Hexagon*>> map; //Definition of map, using array of vectors, where each point is a hexagon

//     public:
//         GameMap(int columns, int rows); //Constructor for the map
//         int GetColumns(void); //Returns the number of columns
//         int GetRows(void); //Returns the number of rows
//         void AddHex(int column, int row); //Adds a hex to the graph
//         void Initialize(void); //Initializes a map to be all base hexes
//         Hexagon* GetHex(int column, int row); //Returns pointer to the hex
//         void ChangeHex(int column, int row, HexagonType type); //Changes the hex based on the type input
//         std::vector<std::vector<int>> ShortestPath(int start_column, int start_row); //Returns the array of distances for a starting point
//         void PrintShortestPath(int start_column, int start_row, int end_column, int end_row); //Prints out the path from a starting point to an ending point (function calls ShortestPath)
//         void Print(void); //Prints out the basic grid form of the map (maybe want to rename to PrintMap)
//         ~GameMap(); //Destructor for the map
// };

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

        std::vector<Hexagon*> GetNeighbors(Hexagon* hexagon);
        std::vector<Hexagon*> PossibleMovements(Hexagon* start, int movement);
        int HexDistance(Hexagon* start, Hexagon* end);
        double Lerp(int a, int b, double t);
        Hexagon* HexLerp(Hexagon* a, Hexagon* b, double t);
        std::vector<Hexagon*> HexLineDraw(Hexagon* start, Hexagon* end, bool& check);

        std::vector<Hexagon*> FieldOfView(Hexagon* start, int range);
        std::vector<Hexagon*> FindClosestPlayer(Hexagon* monster_hexagon, std::vector<Hexagon*> characters_hexagons);
        std::vector<Hexagon*> PathFind(Hexagon* start, Hexagon* end);

        void PrintMap(void);

        ~GameMap();
};

#endif
