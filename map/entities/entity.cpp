#include "entity.h"
#include "../../global.h"
#include <QDebug>
#include "../map.h"

/*
 * Kommer göra dörrar och knappar till Entities som pekas på från tiles.
 *
 * Knappar öppnar dörrar som har samma färg som knappen själv när någonting står på den.
 * Det kommer gå att fråga Map efter alla entities med en viss färg.
 */

Entity::Entity(Map *map)
{
    _map = map;
    _color = ENTITY_COLOR_BLUE;
    _map->addColoredEntity(this);
}

float Entity::height()
{
    return 0.0f;
}

bool Entity::blocksPlayer()
{
    return true;
}

void Entity::playerEntered(int tileX, int tileY)
{

}

void Entity::playerExited(int tileX, int tileY)
{

}

void Entity::movableEntered(int tileX, int tileY)
{

}

void Entity::movableExited(int tileX, int tileY)
{

}

bool Entity::isToggleable()
{
    return false;
}

void Entity::setToggleValue(bool value)
{

}

bool Entity::isButton()
{
    return false;
}

void Rotated::setRotation(Direction rotation)
{
    _rotation = rotation;
}




EntityColor Entity::color()
{
    return _color;
}

void Entity::setColor(EntityColor color)
{

    EntityColor oldColor = _color;
    _color = color;
    if(oldColor != color)
        _map->updateEntityColor(this, oldColor);
}

QColor Entity::qColor()
{
    switch(_color)
    {
    case ENTITY_COLOR_BLUE:
        return QColor(17, 47, 228);
        break;
    case ENTITY_COLOR_RED:
        return QColor(199, 9, 27);
        break;
    case ENTITY_COLOR_GREEN:
        return QColor(11, 193, 71);
        break;
    default:
        return Qt::black;
        break;
    }
}
