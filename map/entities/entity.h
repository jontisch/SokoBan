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
    BUTTON = 2
};
#define N_ENTITIES 3


struct Entity
{
    EntityType type;
    Map *map;
};

struct Door
{
    Entity entity;
    Colored colored;
    Toggleable toggleable;
    Rotated rotated;
};

struct Button
{
    Entity entity;
    Colored colored;
    bool isDown;
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

void initEntity(Entity *entity, Map *map, EntityType type);

void initColored(Colored *colored, EntityColor entityColor);

void initRotated(Rotated *rotated, Direction direction);

void initToggleable(Toggleable *toggleable, bool value);

//Map calls drawAt when it finds an entity at a tile.
void drawEntityAt(Entity *entity, QPainter *painter, QRect renderRect);

    //Returns the height of the entity (1.0 is 1.0*tilesize). Used to calculate the render rect.
float entityHeight(Entity *entity);

    // If this returns true, the player wont be able to walk on the tile it's on.
bool entityBlocksPlayer(Entity *entity);

    //Called by map
void playerEnteredEntity(Entity *entity, int tileX, int tileY);
void playerExitedEntity(Entity *entity, int tileX, int tileY);
void movableEnteredEntity(Entity *entity, int tileX, int tileY);
void movableExitedEntity(Entity *entity, int tileX, int tileY);
    //-


void setToggleValue(Entity *entity, bool value);



QColor EntityColorToQColor(EntityColor color);
void setEntityColor(Entity *entity,  EntityColor color);

bool IsToggleable(Entity *entity);
bool IsRotated(Entity *entity);
bool IsColored(Entity *entity);



#endif // ENTITY_H
