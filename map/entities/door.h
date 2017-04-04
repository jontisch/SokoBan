#ifndef DOOR_H
#define DOOR_H

#include "entity.h"



class Door: public ColoredEntity, public Rotated
{
public:
    Door(Map *map);
    void drawAt(QPainter *painter, QRect tileRect);
    float height();

    //This returns true, because a door is a toggleable. If this returns false, setToggleValue won't be called when buttons change.
    bool isToggleable();

    //Called when button states are changed. Value will be true if all buttons with the door's color are down.
    void setToggleValue(bool value);

    bool blocksPlayer();

private:
    bool _isOpen;
};


#endif // DOOR_H
