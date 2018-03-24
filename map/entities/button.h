#ifndef BUTTON_H
#define BUTTON_H

#include "entity.h"


struct Button
{
    Entity entity;
    Colored colored;
    bool isDown;
};

void InitButton(Button* button, Map *map, int tileX, int tileY, EntityColor color);

void DrawButtonAt(Button *button, QPainter *painter, QRect tileRect);


void SetButtonState(Button *button, bool isDown);


#endif // BUTTON_H
