#include "tile.h"

bool IsValidTileType(TileType type)
{
    switch(type)
    {
    case FLOOR:
    case WALL:
    case WATER:
    case LAVA:
    case SAND:
    case GRASS:
        return true;
    default:
        return false;
    }
}


const char *TileTypeName(TileType type)
{
    switch(type)
    {
    case FLOOR:
        return "Floor";
    case WALL:
        return "Wall";
    case WATER:
        return "Water";
    case LAVA:
        return "Lava";
    case SAND:
        return "Sand";
    case GRASS:
        return "Grass";
    default:
        return "Unknown tile type";
    }
}


PixmapIdentifier PixmapForTileType(TileType type)
{
    switch(type)
    {
    case FLOOR: return PIXMAP_FLOOR; break;
    case WALL: return PIXMAP_WALL; break;
    case WATER: return PIXMAP_WATER; break;
    case LAVA: return PIXMAP_LAVA; break;
    case SAND: return PIXMAP_SAND; break;
    case GRASS: return PIXMAP_GRASS; break;
    default: return PIXMAP_WATER; break;
    }
}

const char *TileFlagName(TileFlag flag)
{
    switch(flag)
    {
    case NO_TILE_FLAGS:
        return "No tile flags";
    case HAS_BOX:
        return "Box";
    case IS_TARGET:
        return "Target";
    case IS_START:
        return "Start";
    case HAS_SNOW:
        return "Snow";
    case WAS_SNOW:
        return "Was snow";
    case HAS_SNOWBALL_SMALL:
        return "Snowball S";
    case HAS_SNOWBALL_MEDIUM:
        return "Snowball M";
    case HAS_SNOWBALL_BIG:
        return "Snowball L";
    default:
        return "Unknown tile flag";
    }
}

PixmapIdentifier PixmapForTileFlag(TileFlag flag)
{
    switch(flag)
    {
    case HAS_BOX:
        return PIXMAP_BOX;
    case IS_TARGET:
        return PIXMAP_TARGET;
    case IS_START:
        return PIXMAP_FLOOR;
    case HAS_SNOW:
        return PIXMAP_SNOW;
    case WAS_SNOW:
        return PIXMAP_EX_SNOW;
    case HAS_SNOWBALL_SMALL:
        return PIXMAP_SNOWBALL_SMALL;
    case HAS_SNOWBALL_MEDIUM:
        return PIXMAP_SNOWBALL_MEDIUM;
    case HAS_SNOWBALL_BIG:
        return PIXMAP_SNOWBALL_BIG;
    default:
        return PIXMAP_WATER;
    }
}

const char *EntityTypeName(EntityType entity)
{
    switch(entity)
    {
    case DOOR_VERTICAL:
        return "Vertical door";
    case DOOR_HORIZONTAL:
        return "Horizontal door";
    case BUTTON:
        return "Button";
    default:
        return "Unknown entity";
    }
}

PixmapIdentifier PixmapForEntity(EntityType entity)
{
    switch(entity)
    {
    case DOOR_VERTICAL:
        return PIXMAP_DOOR_VERTICAL;
    case DOOR_HORIZONTAL:
        return PIXMAP_DOOR_HORIZONTAL;
    case BUTTON:
        return PIXMAP_BUTTON_UP;
    default:
        return PIXMAP_FLOOR;
    }
}

Entity *entityFromEntityType(EntityType entity, Map *map){
    Entity *ret;
    switch(entity)
    {
    case DOOR_VERTICAL:
        ret = new Door(map);
        //ret->setRotation(LEFT);
        break;
    case DOOR_HORIZONTAL:
        ret = new Door(map);
        //ret->setRotation(UP);
        break;
    case BUTTON:
        ret = new Button(map);
        break;
    default:
        break;
    }
    return ret;
}
