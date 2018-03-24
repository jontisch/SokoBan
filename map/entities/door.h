#ifndef DOOR_H
#define DOOR_H

#include "entity.h"

struct Door
{
    Entity entity;
    Colored colored;
    Rotated rotated;
    bool isOpen;
};

void InitDoor(Door *door, Map *map, int tileX, int tileY, EntityColor color, Direction direction);

void DrawDoorAt(Door *door, QPainter *painter, QRect tileRect);

void SetDoorState(Door *door, bool isOpen);

#endif // DOOR_H
