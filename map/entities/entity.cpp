#include "entity.h"

/*
 * Kommer göra dörrar och knappar till Entities som pekas på från tiles.
 *
 * Knappar öppnar dörrar som har samma färg som knappen själv när någonting står på den.
 * Det kommer gå att fråga Map efter alla entities med en viss färg.
 */

Entity::Entity()
{

}


ColoredEntity::ColoredEntity()
{
    _color = ENTITY_COLOR_BLUE;
}
