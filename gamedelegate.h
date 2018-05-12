#ifndef GAMEDELEGATE_H
#define GAMEDELEGATE_H
#include "map/map.h"

class GameDelegate
{
public:
    GameDelegate();
    virtual void mapCompleted(Map *map) = 0;

};

#endif // GAMEDELEGATE_H
