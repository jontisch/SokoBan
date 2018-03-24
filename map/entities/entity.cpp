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

void InitEntity(Entity *entity, Map *map, int tileX, int tileY, EntityType type)
{
    entity->map = map;
    entity->type = type;
    entity->position = QPoint(tileX, tileY);
}

void InitColored(Colored *colored, Entity *entity, EntityColor entityColor)
{
    colored->color = entityColor;
    entity->map->addColoredEntity(entity);
}

void InitRotated(Rotated *rotated, Direction direction)
{
   rotated->direction = direction;
}

void InitToggleable(Toggleable *toggleable, bool value){
    toggleable->value = value;
}

float EntityHeight(Entity *entity)
{
    switch (entity->type) {
    case DOOR_HORIZONTAL:
    case DOOR_VERTICAL:
        return 0.1875f;
    default:
        return 0.0f;
    }
}

bool EntityBlocksPlayer(Entity *entity)
{
    switch(entity->type)
    {
    case DOOR_HORIZONTAL:
    case DOOR_VERTICAL:
        return !((Door *)entity)->isOpen;
    case BUTTON:
        return false;
    default:
        return true;
    }
}

void PlayerEnteredEntity(Entity *entity, int tileX, int tileY)
{
    switch(entity->type)
    {
    case BUTTON:
        SetButtonState((Button *)entity, true);
        break;
    default:
        break;
    }
}

void PlayerExitedEntity(Entity *entity, int tileX, int tileY)
{
    switch(entity->type)
    {
    case BUTTON:
        SetButtonState((Button *)entity, false);
        break;
    default:
        break;
    }
}

void MovableEnteredEntity(Entity *entity, int tileX, int tileY)
{
    switch(entity->type)
    {
    case BUTTON:
        SetButtonState((Button *)entity, true);
        break;
    default:
        break;
    }
}

void MovableExitedEntity(Entity *entity, int tileX, int tileY)
{
    switch(entity->type)
    {
    case BUTTON:
        SetButtonState((Button *)entity, false);
        break;
    default:
        break;
    }
}


void SetEntityColor(Entity *entity, Colored *colored, EntityColor color)
{
    EntityColor oldColor = colored->color;
    colored->color = color;

    if(oldColor != color)
        entity->map->updateEntityColor(entity, oldColor);
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
    switch(entity->type)
    {
    case DOOR_HORIZONTAL:
    case DOOR_VERTICAL:
        return true;
    }
    return false;
}

bool IsRotated(Entity *entity)
{
    switch (entity->type)
    {
    case DOOR_HORIZONTAL:
    case DOOR_VERTICAL:
        return true;
    }
    return false;
}

bool IsColored(Entity *entity)
{
    return true;
}

void DrawEntityAt(Entity *entity, QPainter *painter, QRect renderRect)
{
    switch(entity->type)
    {
    case DOOR_HORIZONTAL:
    case DOOR_VERTICAL:
        DrawDoorAt((Door *)entity, painter, renderRect);
        break;
    case BUTTON:
        DrawButtonAt((Button *)entity, painter, renderRect);
        break;
    default:
        assert(false);
        break;
    }
}

Colored *ColoredComponent(Entity *entity)
{
    switch(entity->type)
    {
    case DOOR_HORIZONTAL:
    case DOOR_VERTICAL:
        return &((Door *)entity)->colored;
    case BUTTON:
        return &((Button *)entity)->colored;
        break;
    default:
        assert(false);
        break;
    }
}

