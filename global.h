#ifndef GLOBAL_H
#define GLOBAL_H

#include <QPixmap>
#include <QBrush>

#define MAP_DIR "/../../SokoBan/SokoBan/maps"

enum PixmapIdentifier
{
    PIXMAP_FLOOR,
    PIXMAP_BOX,
    PIXMAP_WALL,
    PIXMAP_TARGET,
    PIXMAP_WATER,
    PIXMAP_SNOW
};
#define N_PIXMAPS 6


void InitPixmaps();

QPixmap *Pixmap(PixmapIdentifier index);

void *EnlargeMemory(int currentSize, int newSize, void *pointer);

#endif // GLOBAL_H
