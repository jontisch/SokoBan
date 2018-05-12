#include "button.h"
#include <QDebug>
#include <QPainterPath>
#include "../../collections.h"
#include "../map.h"

void InitButton(Button *button, Map *map, int tileX, int tileY, EntityColor color)
{
    InitEntity(&button->entity, map, tileX, tileY, BUTTON);
    InitColored(&button->colored, (Entity *)button, color);
}

void DrawButtonAt(Button *button, QPainter *painter, QRect tileRect)
{
    //TODO This method is probably slow because of all the rects and masks and stuff.

    painter->drawPixmap(tileRect.x(), tileRect.y(), tileRect.width(), tileRect.height(), *Pixmap((button->isDown) ? PIXMAP_BUTTON_DOWN : PIXMAP_BUTTON_UP));
    int spriteSize = (button->isDown) ? 32 : 33;
    int zOffset = (button->isDown) ? 0 : 1;

    //This is the color drawing
    float borderWidth = (float)3/spriteSize * tileRect.width();

    QRectF colorRect(tileRect.x() + (float)4/spriteSize * tileRect.width() - zOffset,
                     tileRect.y() + (float)4/spriteSize * tileRect.height() - zOffset,
                     (float)23/spriteSize * tileRect.width(),
                     (float)23/spriteSize * tileRect.height());

    QColor color = EntityColorToQColor(button->colored.color);

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




void SetButtonState(Button *button, bool isDown)
{
    button->isDown = isDown;
    //TODO Cache the number of buttons somewhere so we don't need to count them every time.


    // Count the number of buttons that are down and have the same color as this button.
    // Then set the toggle value of all toggleables (e.g. doors) to true if there are no unpressed buttons with this color.
    List<Entity *> *entities = button->entity.map->entitiesByColor(button->colored.color);
    int unpressedButtonsFound = 0;
    for(int i = 0; i < entities->N; i++)
    {
        Entity *entity = entities->E[i];

        if(entity->type == BUTTON && !((Button *)entity)->isDown)
        {
            unpressedButtonsFound++;
        }
    }

    bool toggleValue = (unpressedButtonsFound <= 0);
    for(int i = 0; i < entities->N; i++)
    {
        Entity *entity = entities->E[i];

        if(!IsToggleable(entity)) continue;

        if(entity->type == DOOR_HORIZONTAL || entity->type == DOOR_VERTICAL)
        {
            SetDoorState((Door *)entity, toggleValue);
        }
    }
}
