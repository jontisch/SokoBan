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
            int previousFlags;
        } tilesWithFlagChanges[2];
        int nTilesWithFlagChanges;
    };
    static void addTileChange(Move *move, int x, int y, int previousFlags);

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
