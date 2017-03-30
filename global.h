#ifndef GLOBAL_H
#define GLOBAL_H

#include <QPixmap>
#include <QBrush>

#define MAP_DIR "/../../SokoBan/maps"

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
    PIXMAP_EX_SNOW
};
#define N_PIXMAPS 9


void InitPixmaps();

QPixmap *Pixmap(PixmapIdentifier index);

void *EnlargeMemory(int currentSize, int newSize, void *pointer);

char *DuplicateString(char *string);

#endif // GLOBAL_H
