#ifndef TILE_H
#define TILE_H

#include "../global.h"
#include "entities/entity.h"

enum TileFlag {
    NO_TILE_FLAGS = 0,

    HAS_BOX = 1,
    IS_TARGET = 2,
    IS_START = 4,
    HAS_SNOW = 8,
    WAS_SNOW = 16,
    HAS_SNOWBALL_SMALL = 32,
    HAS_SNOWBALL_MEDIUM = 64,
    HAS_SNOWBALL_BIG = 128
};
#define N_TILEFLAGS 8 //Don't count NO_TILE_FLAGS

enum TileType {
    WATER = 0,
    WALL = 2,
    FLOOR = 5,
    LAVA = 7,
    GRASS = 8,
    SAND = 9
};
#define N_TILETYPES 10


const char *TileTypeName(TileType type);
const char *TileFlagName(TileFlag flag);
bool IsValidTileType(TileType type);

struct Tile
{
    TileType type;
    long flags; //Use TileFlag:s
    Entity *interactable;
};

PixmapIdentifier PixmapForTileType(TileType type);
PixmapIdentifier PixmapForTileFlag(TileFlag flag);


#endif // TILE_H
