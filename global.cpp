#include "global.h"
#include "mainwindow.h"


QPixmap **PIXMAPS;

void initPixmaps()
{
    PIXMAPS = (QPixmap **)malloc(sizeof(QPixmap *)*N_TILETYPES);

    PIXMAPS[WATER] = new QPixmap(":/images/watertile.png");

    PIXMAPS[FLOOR] = new QPixmap(":/images/floortile.png");
    PIXMAPS[START] = PIXMAPS[FLOOR];

    PIXMAPS[BOX] = new QPixmap(":/images/boxtile.png");
    PIXMAPS[WALL] = new QPixmap(":/images/walltile.png");
    PIXMAPS[TARGET] = new QPixmap(":/images/targettile.png");

    PIXMAPS[SNOW] = new QPixmap(":/images/snowtile.png");
    PIXMAPS[LAVA] = new QPixmap(":/images/lavatile.png");

}

QPixmap *Pixmap(int index){
    return PIXMAPS[index];
}

const char *tileTypeName(Tiletype type)
{
    switch(type)
    {
    case FLOOR:
        return "Floor";
        break;
    case BOX:
        return "Box";
        break;
    case WALL:
        return "Wall";
        break;
    case TARGET:
        return "Target";
        break;
    case START:
        return "Start";
        break;
    case WATER:
        return "Water";
        break;
    case SNOW:
        return "Snow";
        break;
    case LAVA:
        return "Lava";
        break;
    default:
        return "Unknown tile type";
        break;
    }
}

void *enlargeMemory(int currentSize, int newSize, void *pointer){
    void *temp = malloc(newSize);
    memcpy(temp, pointer, currentSize);
    free(pointer);
    return temp;
}

