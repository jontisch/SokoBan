#include "door.h"


float Door::height()
{
    return 0.1875f;
}

bool Door::isToggleable()
{
    return true;
}

void Door::setToggleValue(bool value)
{
    _isOpen = value;
}

Door::Door(Map *map) : ColoredEntity(map)
{
    _isOpen = false;
}

void Door::drawAt(QPainter *painter, QRect renderRect)
{
    //This is for the color overlay at the top of the door.
    //Calculated for when the door is vertical (_rotation is LEFT or RIGHT)
    QColor color = qColor();

    if(_isOpen) color = Qt::yellow;

    float colorLeft = (float)13/38*renderRect.width();
    float colorTop = (float)1/38*renderRect.width();
    float colorWidth = (float)2/38*renderRect.width();
    float colorHeight = (float)30/38*renderRect.width();

    //There is one pixmap for up and down, and one for left/right
    if(_rotation == UP || _rotation == DOWN)
    {
        painter->drawPixmap(renderRect.x(), renderRect.y(), renderRect.width(), renderRect.height(), *Pixmap(PIXMAP_DOOR_VERTICAL));

        //measurements swapped here cuz rotated.
        painter->fillRect(QRectF(renderRect.x() + colorTop, renderRect.y() + colorLeft, colorHeight, colorWidth), color);
    }
    else
    {
        painter->drawPixmap(renderRect.x(), renderRect.y(), renderRect.width(), renderRect.height(), *Pixmap(PIXMAP_DOOR_HORIZONTAL));
        painter->fillRect(QRectF(renderRect.x() + colorLeft, renderRect.y() + colorTop, colorWidth, colorHeight), color);

    }
}
