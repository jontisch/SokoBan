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
    ENTITY_COLOR_GREEN = 3
};
#define N_ENTITY_COLORS 3

enum EntityType {
    DOOR_VERTICAL = 0,
    DOOR_HORIZONTAL = 1,
    BUTTON = 2
};
#define N_ENTITIES 3


class Entity
{
public:
    Entity(Map *map);

    //Map calls drawAt when it finds an entity at a tile.
    virtual void drawAt(QPainter *painter, QRect renderRect) = 0;

    //Returns the height of the entity (1.0 is 1.0*tilesize). Used to calculate the render rect.
    virtual float height();

    // If this returns true, the player wont be able to walk on the tile it's on.
    virtual bool blocksPlayer();

    //Called by map
    virtual void playerEntered(int tileX, int tileY);
    virtual void playerExited(int tileX, int tileY);
    virtual void movableEntered(int tileX, int tileY);
    virtual void movableExited(int tileX, int tileY);
    //-

    //For example, doors are toggleable. When all buttons with the same color as the toggleable are down,
    // setToggleValue's parameter is true - otherwise it's false.
    virtual bool isToggleable();
    virtual void setToggleValue(bool value);
    //--

    //This should only return true if the object inherits from Button.
    virtual bool isButton();

    virtual EntityType getEntityType() = 0;

    EntityColor color();
    void setColor(EntityColor color);
    QColor qColor();
protected:
    Map *_map;
    EntityColor _color;
};

class Rotated
{
public:
    void setRotation(Direction rotation);

protected:
    Direction _rotation;
};

class ColoredEntity: public Entity
{
public:
    ColoredEntity(Map *map);
    EntityColor color();
    void setColor(EntityColor color);
    QColor qColor();
protected:

};



#endif // ENTITY_H
