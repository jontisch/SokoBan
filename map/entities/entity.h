#ifndef ENTITY_H
#define ENTITY_H

#include <QPainter>
#include <QRect>
#include <QColor>
#include "../../global.h"


class Map;


enum EntityColor
{
    ENTITY_COLOR_BLUE = 1,
    ENTITY_COLOR_RED = 2,
    ENTITY_COLOR_GREEN = 3,

    ENTITY_COLOR_NONE = 0
};
#define N_ENTITY_COLORS 3

enum EntityType {
    DOOR_VERTICAL = 0,
    DOOR_HORIZONTAL = 1,
    BUTTON = 2,
    MONSTER = 3
};
#define N_ENTITIES 4


struct Entity
{
    EntityType type;
    Map *map;
    QPoint position;
};

struct Colored
{
    EntityColor color;
};

struct Toggleable
{
    bool value;
};

struct Rotated
{
    Direction direction;
};

void InitEntity(Entity *entity, Map *map, int tileX, int tileY, EntityType type);

void InitColored(Colored *colored, Entity *entity, EntityColor entityColor);

void InitRotated(Rotated *rotated, Direction direction);

//Map calls drawAt when it finds an entity at a tile.
void DrawEntityAt(Entity *entity, QPainter *painter, QRect renderRect);

    //Returns the height of the entity (1.0 is 1.0*tilesize). Used to calculate the render rect.
float EntityHeight(Entity *entity);

    // If this returns true, the player wont be able to walk on the tile it's on.
bool EntityBlocksPlayer(Entity *entity);

    //Called by map
void PlayerEnteredEntity(Entity *entity, int tileX, int tileY);
void PlayerExitedEntity(Entity *entity, int tileX, int tileY);
void MovableEnteredEntity(Entity *entity, int tileX, int tileY);
void MovableExitedEntity(Entity *entity, int tileX, int tileY);
    //-


void setToggleValue(Entity *entity, bool value);

void MoveEntity(Entity *entity, Map *map, QPoint newPosition);

QColor EntityColorToQColor(EntityColor color);
void SetEntityColor(Entity *entity,  EntityColor color);

bool IsToggleable(Entity *entity);
bool IsRotated(Entity *entity);
bool IsColored(Entity *entity);

Colored *ColoredComponent(Entity *entity);

void MakeEntityMove(Entity *entity, Map *map);

#endif // ENTITY_H
