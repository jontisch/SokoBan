#ifndef MOVE_STACK_H
#define MOVE_STACK_H

#include "global.h"
#include "map.h"

class Map;

class MoveStack
{
public:
    MoveStack();

    struct Move
    {
        int playerDX;
        int playerDY;

        struct
        {
            int x;
            int y;
            Tiletype previousType;
        } changedTiles[2];
        int nChangedTiles;
    };

    void pushMove(Move move);
    void pushMove(int playerDX,
                  int playerDY);

    int movesDone();
    bool revertMove(Map *map);

    ~MoveStack();

private:


    int allocatedMoves;
    int nMoves;
    Move *moves;
};

#endif // MOVE_STACK_H
