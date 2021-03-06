#ifndef TILE_H
#define TILE_H

#include "../global.h"
#include "entities/entity.h"
#include "entities/door.h"
#include "entities/button.h"
#include "entities/monster.h"
#include "collections.h"

enum TileDecoration
{
    NO_DECORATION = 0,
    CACTUS
};

enum TileEffect
{
    NO_EFFECT = 0,
    EXPLOSION = 1,
    WHIRLWIND = 2
};

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
const char *EntityTypeName(EntityType entity);


Entity *EntityFromEntityType(EntityType entity, EntityColor color, Map *map, int tileX, int tileY);


bool IsValidTileType(TileType type);

struct Tile
{
    TileType type;
    long flags; //Use TileFlag:s
    List<Entity *> entities = {0};
    TileDecoration decoration;
    int tick;
    TileEffect effect;
    int effectTick;
};

PixmapIdentifier PixmapForTileType(TileType type);
PixmapIdentifier PixmapForTileFlag(TileFlag flag);
PixmapIdentifier PixmapForEntity(EntityType entity);
PixmapIdentifier PixmapForDecoration(TileDecoration decoration);

#endif // TILE_H
