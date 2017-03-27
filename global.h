#ifndef GLOBAL_H
#define GLOBAL_H

#include <QPixmap>
#include <QBrush>

#define MAP_DIR "/../../SokoBan/SokoBan/maps"

enum Tiletype {
    FLOOR = 0,
    BOX = 1,
    WALL = 2,
    TARGET = 3,
    START = 4,
    WATER = 5
};
#define N_TILETYPES 6

void initPixmaps();

QPixmap *Pixmap(int index);

const char *tileTypeName(Tiletype type);

void *enlargeMemory(int currentSize, int newSize, void *pointer);

#endif // GLOBAL_H
