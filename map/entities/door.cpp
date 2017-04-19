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

bool Door::blocksPlayer()
{
    return (!_isOpen); //TODO @Bug If you have a button in front of the door and you stand on it, you can enter the door tile and you'll be standing on a closed door.
}

EntityType Door::getEntityType()
{
    if (_rotation == UP  || _rotation == DOWN)
        return DOOR_VERTICAL;
    else
        return DOOR_HORIZONTAL;
}

Door::Door(Map *map) : Entity(map)
{
    _isOpen = false;
}

void Door::drawAt(QPainter *painter, QRect renderRect)
{
    //This is for the color overlay at the top of the door.
    //Calculated for when the door is vertical (_rotation is LEFT or RIGHT)
    QColor color = qColor();

    float zOffset = (_isOpen) ? height() * renderRect.width() : 0;

    float colorLeft = (float)13/38*renderRect.width();
    float colorTop = (float)1/38*renderRect.width();
    float colorWidth = (float)2/38*renderRect.width();
    float colorHeight = (float)30/38*renderRect.width();

    float maskWidth = (float)26/38*renderRect.width();
    float maskHeight = renderRect.width();

    if(_rotation == UP || _rotation == DOWN)
    {
        float tmp = maskHeight;
        maskHeight = maskWidth;
        maskWidth = tmp;
    }

    painter->save();

    painter->setClipRect(QRectF(renderRect.x(), renderRect.y(), maskWidth, maskHeight));

    //There is one pixmap for up and down, and one for left/right
    if(_rotation == UP || _rotation == DOWN)
    {
        painter->drawPixmap(renderRect.x() + zOffset, renderRect.y() + zOffset, renderRect.width(), renderRect.height(), *Pixmap(PIXMAP_DOOR_VERTICAL));

        //measurements swapped here cuz rotated.
        painter->fillRect(QRectF(renderRect.x() + zOffset + colorTop, renderRect.y() + zOffset + colorLeft, colorHeight, colorWidth), color);
    }
    else
    {
        painter->drawPixmap(renderRect.x() + zOffset, renderRect.y() + zOffset, renderRect.width(), renderRect.height(), *Pixmap(PIXMAP_DOOR_HORIZONTAL));
        painter->fillRect(QRectF(renderRect.x() + zOffset + colorLeft, renderRect.y() + zOffset + colorTop, colorWidth, colorHeight), color);

    }

    painter->restore();
}
