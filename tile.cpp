#include "tile.h"

bool IsValidTileType(TileType type)
{
    switch(type)
    {
    case FLOOR:
    case WALL:
    case WATER:
    case SNOW:
    case LAVA:
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
    case SNOW:
        return "Snow";
    case LAVA:
        return "Lava";
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
    case SNOW: return PIXMAP_SNOW; break;
    case LAVA: return PIXMAP_LAVA; break;
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
    default:
        return "Unknown tile flag";
    }
}
