#ifndef DOOR_H
#define DOOR_H

#include "entity.h"



class Door: public Entity, public Rotated, public Colored, public Toggleable
{
public:
    Door(Map *map, EntityColor color);
    void drawAt(QPainter *painter, QRect tileRect);
    float height();

    bool blocksPlayer();

    void setToggleValue(bool value);

    EntityType getEntityType();
    bool isToggleable();
    bool isRotated();
    bool isColored();

private:
    bool _isOpen;
};


#endif // DOOR_H
