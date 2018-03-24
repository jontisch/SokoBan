#include "entity.h"
#include "../../global.h"
#include <QDebug>
#include "../map.h"
#include "assert.h"

/*
 * Kommer göra dörrar och knappar till Entities som pekas på från tiles.
 *
 * Knappar öppnar dörrar som har samma färg som knappen själv när någonting står på den.
 * Det kommer gå att fråga Map efter alla entities med en viss färg.
 */

void initEntity(Entity *entity, Map *map, EntityType type)
{
    entity->map = map;
    entity->type = type;
}

void initColored(Colored *colored, EntityColor entityColor)
{
    colored->color = entityColor;
}

void initRotated(Rotated *rotated, Direction direction)
{
   rotated->direction = direction;
}

void initToggleable(Toggleable *toggleable, bool value){
    toggleable->value = value;
}

float entityHeight(Entity *entity)
{
    switch (entity->type) {
    case DOOR_HORIZONTAL:
    case DOOR_VERTICAL:
        return 0.1875f;
    default:
        return 0.0f;
    }


}

bool entityBlocksPlayer(Entity *entity)
{
    return true;
}

void playerEntered(Entity *entity, int tileX, int tileY)
{
    switch(entity->type)
    {
    case BUTTON:
        ((Button *)entity)->isDown = true;
        break;
    default:
        break;
    }
}

void playerExited(Entity *entity, int tileX, int tileY)
{
    switch(entity->type)
    {
    case BUTTON:
        ((Button *)entity)->isDown = false;
        break;
    default:
        break;
    }
}

void movableEntered(Entity *entity, int tileX, int tileY)
{
    switch(entity->type)
    {
    case BUTTON:
        ((Button *)entity)->isDown = true;
        break;
    default:
        break;
    }
}

void movableExited(Entity *entity, int tileX, int tileY)
{
    switch(entity->type)
    {
    case BUTTON:
        ((Button *)entity)->isDown = false;
        break;
    default:
        break;
    }
}


void Toggleable::setToggleValue(Entity *entity, bool value)
{

}


void setEntityColor(Entity *entity, EntityColor color)
{

    EntityColor oldColor = _color;
    _color = color;

    Entity *entity = reinterpret_cast<Entity *>(this);
    if(oldColor != color)
        entity->_map->updateEntityColor(entity, oldColor);
}

QColor EntityColorToQColor(EntityColor color)
{
    switch(color)
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


bool IsToggleable(Entity *entity)
{
    if(dynamic_cast<Door *>(entity)) return true;
    return false;
}

bool IsRotated(Entity *entity)
{
    if(dynamic_cast<Door *>(entity)) return true;
    return false;
}

bool IsColored(Entity *entity)
{
    return true;
}

void drawEntityAt(Entity *entity, QPainter *painter, QRect renderRect)
{

}
