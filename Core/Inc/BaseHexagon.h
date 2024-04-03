/*
MOST RECENT AUTHOR: JACKSON
DATE: 10/31/2023
*/

#ifndef BASEHEXAGON_HH
#define BASEHEXAGON_HH

#include "Hexagon.h"

class Hexagon {
    protected:
        int _q;
        int _r;
        int _row;
        int _column;
        HexagonType _type;
        bool _passable;

    public:
        Hexagon(int q, int r);

        int GetHexQ(void);
        int GetHexR(void);
        int GetHexRow(void);
        int GetHexColumn(void);
        HexagonType GetType(void);
        bool GetPassable(void);

        void SetHexQ(int q);
        void SetHexR(int r);
        void SetHexRow(int row);
        void SetHexColumn(int column);
        void SetType(HexagonType type);
        void SetPassable(bool passable);

        ~Hexagon();
};

#endif
