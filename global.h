#ifndef GLOBAL_H
#define GLOBAL_H

#include <QPixmap>
#include <QBrush>

#define MAP_DIR "/../../SokoBan/maps"

enum Direction
{
    LEFT,
    RIGHT,
    UP,
    DOWN
};

int XModifiedByDirection(int x, Direction direction, int delta = 1);
int YModifiedByDirection(int y, Direction direction, int delta = 1);


enum PixmapIdentifier
{
    NO_PIXMAP,
    PIXMAP_FLOOR,
    PIXMAP_BOX,
    PIXMAP_WALL,
    PIXMAP_TARGET,
    PIXMAP_WATER,
    PIXMAP_SNOW,
    PIXMAP_LAVA,
    PIXMAP_EX_SNOW,
    PIXMAP_SNOWBALL_SMALL,
    PIXMAP_SNOWBALL_MEDIUM,
    PIXMAP_SNOWBALL_BIG,
    PIXMAP_ICE
};
#define N_PIXMAPS 11


void InitPixmaps();

QPixmap *Pixmap(PixmapIdentifier index);

void *EnlargeMemory(int currentSize, int newSize, void *pointer);

char *DuplicateString(char *string);

#endif // GLOBAL_H
