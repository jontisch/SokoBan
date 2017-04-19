#include "button.h"
#include <QDebug>
#include <QPainterPath>
#include "../../collection.h"
#include "../map.h"


Button::Button(Map *map): Entity(map)
{
    _isDown = false;
}

void Button::drawAt(QPainter *painter, QRect tileRect)
{
    //TODO This method is probably slow because of all the rects and masks and stuff.

    painter->drawPixmap(tileRect.x(), tileRect.y(), tileRect.width(), tileRect.height(), *Pixmap((_isDown) ? PIXMAP_BUTTON_DOWN : PIXMAP_BUTTON_UP));
    int spriteSize = (_isDown) ? 32 : 33;
    int zOffset = (_isDown) ? 0 : 1;

    //This is the color drawing
    float borderWidth = (float)3/spriteSize * tileRect.width();

    QRectF colorRect(tileRect.x() + (float)4/spriteSize * tileRect.width() - zOffset,
                     tileRect.y() + (float)4/spriteSize * tileRect.height() - zOffset,
                     (float)23/spriteSize * tileRect.width(),
                     (float)23/spriteSize * tileRect.height());

    QColor color = qColor();

    painter->fillRect(colorRect.x(),
                      colorRect.y(),
                      borderWidth,
                      colorRect.height(), color);

    painter->fillRect(colorRect.x(),
                      colorRect.y(),
                      colorRect.width(),
                      borderWidth, color);

    painter->fillRect(colorRect.x() + colorRect.width() - borderWidth,
                      colorRect.y(),
                      borderWidth,
                      colorRect.height(), color);

    painter->fillRect(colorRect.x(),
                      colorRect.y() + colorRect.height() - borderWidth,
                      colorRect.width(),
                      borderWidth, color);

    //--

}

bool Button::blocksPlayer()
{
    return false;
}

void Button::playerEntered(int tileX, int tileY)
{
    setIsDown(true);
}

void Button::playerExited(int tileX, int tileY)
{
    setIsDown(false);
}

void Button::movableEntered(int tileX, int tileY)
{
    setIsDown(true);
}

void Button::movableExited(int tileX, int tileY)
{
    setIsDown(false);
}

void Button::setIsDown(bool value)
{
    _isDown = value;
    //TODO Cache the number of buttons somewhere so we don't need to count them every time.


    // Count the number of buttons that are down and have the same color as this button.
    // Then set the toggle value of all toggleables (e.g. doors) to true if there are no unpressed buttons with this color.
    Collection<Entity *> *entities = _map->entitiesByColor(_color);
    int unpressedButtonsFound = 0;
    for(int i = 0; i < entities->size(); i++)
    {
        Entity *entity;
        if(entities->get(i, &entity) && entity->isButton())
        {
            qDebug() << ((Button*)entity)->isDown();
            if(!((Button*)entity)->isDown())
            {
                unpressedButtonsFound++;
            }
        }
    }

    bool toggleValue = (unpressedButtonsFound <= 0);
    for(int i = 0; i < entities->size(); i++)
    {
        Entity *entity;
        if(entities->get(i, &entity))
        {
            if(entity->isToggleable())
            {
                entity->setToggleValue(toggleValue);
            }
        }
    }
}

bool Button::isButton()
{
    return true;
}

bool Button::isDown()
{
    return _isDown;
}

EntityType Button::getEntityType()
{
    return BUTTON;
}
