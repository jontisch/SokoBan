#ifndef BUTTON_H
#define BUTTON_H

#include "entity.h"


class Button: public ColoredEntity
{
public:
    Button(Map *map);
    void drawAt(QPainter *painter, QRect tileRect);

    bool blocksPlayer();

    void playerEntered(int tileX, int tileY);
    void playerExited(int tileX, int tileY);
    void movableEntered(int tileX, int tileY);
    void movableExited(int tileX, int tileY);

    void setIsDown(bool value);

    // Returns true, because this is a button.
    bool isButton();

    bool isDown();
private:
    bool _isDown;
};

#endif // BUTTON_H
