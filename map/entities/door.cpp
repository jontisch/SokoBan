#include "door.h"
#include "../tile.h"
#include "../map.h"


void InitDoor(Door *door, Map *map, int tileX, int tileY, EntityColor color, Direction direction)
{
    InitEntity(&door->entity, map, tileX, tileY, (direction == UP || direction == DOWN) ? DOOR_VERTICAL : DOOR_HORIZONTAL);
    InitColored(&door->colored, (Entity *)door, color);
    InitRotated(&door->rotated, direction);
    door->isOpen = false;
}

void DrawDoorAt(Door *door, QPainter *painter, QRect tileRect)
{
    //This is for the color overlay at the top of the door.
    //Calculated for when the door is vertical (_rotation is LEFT or RIGHT)
    QColor color = EntityColorToQColor(door->colored.color);

    float zOffset = (door->isOpen) ? EntityHeight((Entity *)door) * tileRect.width() : 0;

    float colorLeft = (float)13/38*tileRect.width();
    float colorTop = (float)1/38*tileRect.width();
    float colorWidth = (float)2/38*tileRect.width();
    float colorHeight = (float)30/38*tileRect.width();

    float maskWidth = (float)26/38*tileRect.width();
    float maskHeight = tileRect.width();

    Direction dir = door->rotated.direction;

    if(dir == UP || dir == DOWN)
    {
        float tmp = maskHeight;
        maskHeight = maskWidth;
        maskWidth = tmp;
    }

    painter->save();

    painter->setClipRect(QRectF(tileRect.x(), tileRect.y(), maskWidth, maskHeight));

    //There is one pixmap for up and down, and one for left/right
    if(dir == UP || dir == DOWN)
    {
        painter->drawPixmap(tileRect.x() + zOffset, tileRect.y() + zOffset, tileRect.width(), tileRect.height(), *Pixmap(PIXMAP_DOOR_VERTICAL));

        //measurements swapped here cuz rotated.
        painter->fillRect(QRectF(tileRect.x() + zOffset + colorTop, tileRect.y() + zOffset + colorLeft, colorHeight, colorWidth), color);
    }
    else
    {
        painter->drawPixmap(tileRect.x() + zOffset, tileRect.y() + zOffset, tileRect.width(), tileRect.height(), *Pixmap(PIXMAP_DOOR_HORIZONTAL));
        painter->fillRect(QRectF(tileRect.x() + zOffset + colorLeft, tileRect.y() + zOffset + colorTop, colorWidth, colorHeight), color);

    }

    painter->restore();
}

void SetDoorState(Door *door, bool isOpen)
{
    Map *map = door->entity.map;


    if(!map->tileHasMovable(door->entity.position.x(), door->entity.position.y()))
    {
        if(map->_player != door->entity.position)
        {
            door->isOpen = isOpen;
        }
    }
}
