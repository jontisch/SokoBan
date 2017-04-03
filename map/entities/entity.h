#ifndef ENTITY_H
#define ENTITY_H

enum EntityColor
{
    ENTITY_COLOR_BLUE = 1,
    ENTITY_COLOR_RED = 2,
    ENTITY_COLOR_GREEN = 3
};

class Entity
{

};

class ColoredEntity
{
public:
    ColoredEntity();
    EntityColor color();
    void setColor(EntityColor color);
private:
    EntityColor _color;
};

class Door: ColoredEntity
{
private:
    bool _isOpen;
};

class Button: ColoredEntity
{

};

#endif // ENTITY_H
