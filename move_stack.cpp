#include "move_stack.h"

void MoveStack::addTileChange(Move *move, int x, int y, int previousFlags)
{
    move->tilesWithFlagChanges[move->nTilesWithFlagChanges].x = x;
    move->tilesWithFlagChanges[move->nTilesWithFlagChanges].y = y;
    move->tilesWithFlagChanges[move->nTilesWithFlagChanges].previousFlags = previousFlags;
    move->nTilesWithFlagChanges++;
}


MoveStack::MoveStack()
{
    allocatedMoves = 128;
    moves = (Move *)malloc(sizeof(Move)*allocatedMoves);
    nMoves = 0;
}


void MoveStack::pushMove(Move move)
{
    nMoves += 1;
    int newAllocationSize = allocatedMoves;
    while(newAllocationSize <= nMoves)
    {
        newAllocationSize *= 2;
    }
    if(newAllocationSize != allocatedMoves)
    {
        Move *newMoves = (Move *)malloc(sizeof(Move)*newAllocationSize);
        memcpy(newMoves, moves, sizeof(Move)*allocatedMoves);
        free(moves);
        moves = newMoves;
        allocatedMoves = newAllocationSize;
    }
    moves[nMoves-1] = move;
}

void MoveStack::pushMove(int playerDX,
                         int playerDY)
{
    Move move;

    move.playerDX = playerDX;
    move.playerDY = playerDY;
    move.nTilesWithFlagChanges = 0;

    pushMove(move);
}


int MoveStack::movesDone()
{
    return nMoves;
}

bool MoveStack::revertMove(Map *map)
{
    if(nMoves <= 0) return false;

    Move *move = &moves[nMoves-1];
    map->movePlayer(-move->playerDX, -move->playerDY, true);

    for(int i = 0; i < move->nTilesWithFlagChanges; i++)
    {
        auto t = &move->tilesWithFlagChanges[i];
        map->setTileFlags(t->x, t->y, t->previousFlags);
    }
    nMoves--;

    return (nMoves > 0);
}

MoveStack::~MoveStack()
{
    free(moves);
}
