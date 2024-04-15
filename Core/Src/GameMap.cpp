#include "GameMap.h"

GameMap::GameMap(int rows, int columns) : _rows(rows), _columns(columns) {
    map.resize(rows, std::vector<Hexagon*>(columns, nullptr));

    for (int r = 0; r < rows; r++) {
        int r_offset = -1 * (r / 2);
        for (int q = r_offset; q < columns + r_offset; q++) {
            map[r][q + floor(r / 2)] = new Hexagon(q, r);
        }
    }
}

GameMap::GameMap(std::string mapstring) {
    _rows = _columns = int(sqrt(mapstring.length()) + 0.5);

    map.resize(_rows, std::vector<Hexagon*>(_columns, nullptr));
    for (int r = 0; r < _rows; r++) {
        int r_offset = -1 * (r / 2);
        for (int q = r_offset; q < _columns + r_offset; q++) {
            map[r][q + floor(r / 2)] = new Hexagon(q, r);
        }
    }

    for (int i = 0; i < _columns; i++) {
        for (int j = 0; j < _rows; j++) {
            ChangeHex(i, j, static_cast<HexagonType> ((int) mapstring[_rows*i + j] - 48));
        }
    }
}

//Returns the number of rows
int GameMap::GetRows(void) {
    return _rows;
}

//Returns the number of columns
int GameMap::GetColumns(void) {
    return _columns;
}

//Returns pointer to the hex
Hexagon* GameMap::GetHex(int row, int column) {
    if (row >= 0 && row < _rows && column >= 0 && column < _columns) {
        return map[row][column];
    }
    else {
        return nullptr;
    }
}

void GameMap::ChangeHex(int row, int column, HexagonType type) {
    Hexagon* hex = GetHex(row, column); //Retrieves hex from a position
    hex->SetType(type); //Sets the new type of the position
    if (type == BaseHex) {
        hex->SetPassable(true); //Sets to passable is desired type is base
    }
    else {
        hex->SetPassable(false); //Sets to impassable is desired type is not base
    }
}

//utility function to retrieve the neighbors of a hexagon using q and r coordinates
std::vector<Hexagon*> GameMap::GetNeighbors(Hexagon* hexagon) {
    std::vector<Hexagon*> neighbors;

    //gets the input hexagon's q and r values
    int q = hexagon->GetHexQ();
    int r = hexagon->GetHexR();

    //creates vectors for the q and r values of the neighboring hexagons
    const std::vector<int> dq = {1, 1, 0, -1, -1, 0};
    const std::vector<int> dr = {0, -1, -1, 0, 1, 1};


    //iterates through each neighboring hexagon
    for (int i = 0; i < 6; ++i) {
        int neighbor_q = q + dq[i];
        int neighbor_r = r + dr[i];
        int row = neighbor_r;
        int column = neighbor_q + floor(neighbor_r / 2);

        //checks if the neighboring hexagon is within the bounds of the map
        //if it is, then it adds the hexagon to the neighbors vector
        if (row >= 0 && row < _rows && column >= 0 && column < _columns) {
            neighbors.push_back(GetHex(row, column));
        }
    }

    return neighbors;
}

std::vector<Hexagon*> GameMap::PossibleMovements(Hexagon* start, int movement) {
    std::queue<std::pair<Hexagon*, int>> queue;
    std::vector<Hexagon*> visited;
    queue.push({start, 0});
    //visited.push_back(start);

    while (!queue.empty()) {
        Hexagon* current = queue.front().first;
        int current_movement = queue.front().second;
        queue.pop();
        std::vector<Hexagon*> neighbors = GetNeighbors(current);

        if (current_movement >= movement) {
            continue;
        }

        for (Hexagon* neighbor : neighbors) {
            if (std::find(visited.begin(), visited.end(), neighbor) == visited.end() && neighbor->GetPassable() == true) {
                queue.push({neighbor, current_movement + 1});
                visited.push_back(neighbor);
            }
        }
    }

    return visited;
}

//reconstructs the path from the end node to the start node by visiting the parent of each node
//utility function for the A* algorithm
std::vector<Hexagon*> ReconstructPath(AStarNode* end_node) {
    std::vector<Hexagon*> path;
    AStarNode* current = end_node;
    while (current != nullptr) {
        path.push_back(current->hex);
        current = current->parent;
    }
    std::reverse(path.begin(), path.end());
    return path;
}

//utility function to calculate the distance between two hexagons
//used as the heuristic calculation for the A* algorithm
int GameMap::HexDistance(Hexagon* a, Hexagon* b) {
    return ((abs(a->GetHexQ() - b->GetHexQ()) + abs(a->GetHexQ() + a->GetHexR() - b->GetHexQ() - b->GetHexR()) + abs(a->GetHexR() - b->GetHexR())) / 2);
}

double GameMap::Lerp(double a, double b, double t) {
    return a + (b - a) * t;
}

std::pair<double, double> GameMap::HexLerp(Hexagon* a, Hexagon* b, double t) {
    double q = Lerp(a->GetHexQ(), b->GetHexQ(), t);
    double r = Lerp(a->GetHexR(), b->GetHexR(), t);
    std::pair<double, double> coordinates = {q, r};

    return coordinates;
}

std::vector<double> GameMap::AxialCube(std::pair<double, double> coordinates) {
    double q = coordinates.first - 0.000003;
    double r = coordinates.second + 0.000002;
    double s = -q - r + 0.000001;
    return std::vector<double>{q, r, s};
}

Hexagon* GameMap::HexRound(std::vector<double> coordinates) {
    int q = round(coordinates[0]);
    int r = round(coordinates[1]);
    int s = round(coordinates[2]);
    double q_diff = abs(q - coordinates[0]);
    double r_diff = abs(r - coordinates[1]);
    double s_diff = abs(s - coordinates[2]);
    if (q_diff > r_diff && q_diff > s_diff) {
        q = -r - s;
    }

    else if (r_diff > s_diff) {
        r = -q - s;
    }
    else {
        s = -q -r;
    }

    int row = r;
    int col = q + floor(r / 2);

    return GetHex(row, col);
}

std::vector<Hexagon*> GameMap::HexLineDraw(Hexagon* start, Hexagon* end) {
    int distance = HexDistance(start, end);
    std::vector<Hexagon*> line;
    for (int i = 0; i <= distance; i++) {
        double t = 1.0 / distance * i;
        Hexagon* output = HexRound(AxialCube(HexLerp(start, end, t)));
        if (output != NULL) {
            if (output->GetType() == WallHex) {
            	line.push_back(output);
                break;
            }
            else {
                line.push_back(output);
            }
        }
    }

    return line;
}

// Function to calculate the field of view
std::vector<Hexagon*> GameMap::FieldOfView(Hexagon* start, int range) {
	std::vector<Hexagon*>visible_hexes;
    std::set<Hexagon*> unique_hexes;
    for (int row = 0; row < _rows; row++) {
        for (int col = 0; col < _columns; col++) {
            Hexagon* hexagon = GetHex(row, col);
            if(hexagon == start) {
            	continue;
            }
            if (HexDistance(start, hexagon) <= range) {
                std::vector<Hexagon*> line = HexLineDraw(start, hexagon);
                if (line.size() != 0) {
                    unique_hexes.insert(line.begin(), line.end());
                }
            }
        }
    }
    visible_hexes.insert(visible_hexes.end(), unique_hexes.begin(), unique_hexes.end());

    return visible_hexes;
}

std::vector<Hexagon*> GameMap::PathFind(Hexagon* start, Hexagon* end) {

    //creates a lambda expression to compare the f cost of two nodes
    auto Compare = [](AStarNode* node1, AStarNode* node2) {
        return node1->GetFCost() > node2->GetFCost();
    };

    //initializes the data structures necessary for the A* algorithm
    //open_set is a priority queue that sorts the nodes based on their f cost
    //closed_set is a set that keeps track of the nodes that have been visited
    //node_map is a map that keeps track of each hex and its corresponding A* data
    std::priority_queue<AStarNode*, std::vector<AStarNode*>, decltype(Compare)> open_set(Compare);
    std::unordered_set<Hexagon*> closed_set;
    std::unordered_map<Hexagon*, AStarNode*> node_map;

    //creates the starting A* node and pushes it onto the open set
    AStarNode* start_node = new AStarNode(start, nullptr, 0, HexDistance(start, end));
    open_set.push(start_node);

    //also stores the starting node in the node map
    node_map[start] = start_node;

    //runs the A* algorithm until all nodes have been visited
    while (!open_set.empty()) {

        //starts by taking the node with the lowest f cost from the open set
        AStarNode* current = open_set.top();
        open_set.pop();

        //if the popped node is the end node, the algorithm traces back through the path and returns it
        if (current->hex == end) {
            std::vector<Hexagon*> path = ReconstructPath(current);
            for (const auto& pair : node_map) {
                if (pair.second != nullptr) {
                    delete pair.second;
                }
            }
            return path;
        }

        //otherwise, the algorithm continues by visiting the neighbors of the current node
        //and adding the current node to the closed set
        closed_set.insert(current->hex);

        //visits all the neighbors of the current node
        for (Hexagon* neighbor : GetNeighbors(current->hex)) {
            //if the neighbor is passable and has not been visited, enters conditional
            if (neighbor->GetPassable() && closed_set.find(neighbor) == closed_set.end()) {
                int tentative_g_cost = current->g_cost + 1;

                //creates a new A* node for the neighbor if it has not been visited
                AStarNode* neighbor_node = nullptr;
                auto it = node_map.find(neighbor);
                if (it == node_map.end()) {
                    //calculates the heuristic cost for the neighbor
                    int h_cost = HexDistance(neighbor, end);
                    //creates the new A* node and pushes it onto the open set
                    neighbor_node = new AStarNode(neighbor, current, tentative_g_cost, h_cost);
                    open_set.push(neighbor_node);
                    node_map[neighbor] = neighbor_node;
                }
                else {
                    //if the node is already on the node map, updates the g cost and parent of the node
                    neighbor_node = it->second;
                    if (tentative_g_cost < neighbor_node->g_cost) {
                        neighbor_node->g_cost = tentative_g_cost;
                        neighbor_node->parent = current;
                    }
                }
            }
        }
    }

    //destroys the node_map before returning an empty vector
    //this is necessary to avoid memory leaks
    //an empty vector is returned if the end node is not reachable
    for (const auto& pair : node_map) {
        if (pair.second != nullptr) {
            delete pair.second;
        }
    }

    return {};
}

void GameMap::PrintMap(void) {
    for (int row = 0; row < _rows; row++) {
        if (row % 2 == 1) {
            std::cout << "        ";
        }
        for (int column = 0; column < _columns; column++) {
            HexagonType type = map[row][column]->GetType();
            if (type == BaseHex) {
                std::string output = "| Base (";
                std::cout << output << map[row][column]->GetHexQ() << " " << map[row][column]->GetHexR() << ") |";
            }
            else if (type == WallHex) {
                std::string output = "| Wall (";
                std::cout << output << map[row][column]->GetHexQ() << " " << map[row][column]->GetHexR() << ") |";
            }
            else if (type == PlayerHex) {
                std::string output = "| Player (";
                std::cout << output << map[row][column]->GetHexQ() << " " << map[row][column]->GetHexR() << ") |";
            }
            else if (type == MonsterHex) {
                std::string output = "| Monster (";
                std::cout << output << map[row][column]->GetHexQ() << " " << map[row][column]->GetHexR() << ") |";
            }
            else {
                std::string output = "Unknown ";
                std::cout << output << map[row][column]->GetHexQ() << " " << map[row][column]->GetHexR() << " ";
            }
        }
        std::cout << "\n" << std::endl;
    }
}

//Destructor for the map
GameMap::~GameMap() {
    //Iterates through each position of the map
    for (int row = 0; row < _rows; row++) {
        for (int column = 0; column < _columns; column++) {
            //Checks to see if the map has a hex (it should always pass this)
            if (map[row][column] != nullptr) {
                delete map[row][column]; //Deletes hex
            }
        }
    }
}
