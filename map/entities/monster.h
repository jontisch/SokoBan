#ifndef MONSTER_H
#define MONSTER_H

#include "entity.h"
#include <QPainter>

struct Monster
{
    Entity entity;
    Direction walkDirection;
};


void InitMonster(Monster *monster, Map *map, int tileX, int tileY);
void DrawMonsterAt(Monster *monster, QPainter *painter, QRect tileRect);

void MakeMonsterMove(Monster *monster, Map *map);

#endif // MONSTER_H
