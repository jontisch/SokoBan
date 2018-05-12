
#include "monster.h"


void InitMonster(Monster *monster, Map *map, int tileX, int tileY)
{
    InitEntity(&monster->entity, map, tileX, tileY, MONSTER);
    monster->walkDirection = RandomDirection();
}

void DrawMonsterAt(Monster *monster, QPainter *painter, QRect tileRect)
{
    QPixmap *pixmap = Pixmap(PIXMAP_MONSTER);
    painter->drawPixmap(tileRect, *pixmap);
}


void MakeMonsterMove(Monster *monster, Map *map)
{
    MoveEntity((Entity *)monster, map, QPoint(0, 0));
}
