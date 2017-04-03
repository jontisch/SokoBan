#include "global.h"
#include "mainwindow.h"


QPixmap **PIXMAPS;


int XModifiedByDirection(int x, Direction direction, int delta)
{
    switch(direction)
    {
    case LEFT:
        return x - delta;
    case RIGHT:
        return x + delta;
    default:
        return x;
    }
}

int YModifiedByDirection(int y, Direction direction, int delta)
{
    switch(direction)
    {
    case UP:
        return y - delta;
    case DOWN:
        return y + delta;
    default:
        return y;
    }
}


void InitPixmaps()
{
    PIXMAPS = (QPixmap **)malloc(sizeof(QPixmap *)*N_PIXMAPS);

    PIXMAPS[PIXMAP_WATER] = new QPixmap(":/images/watertile.png");

    PIXMAPS[PIXMAP_FLOOR] = new QPixmap(":/images/floortile.png");

    PIXMAPS[PIXMAP_BOX] = new QPixmap(":/images/boxtile.png");
    PIXMAPS[PIXMAP_WALL] = new QPixmap(":/images/walltile.png");
    PIXMAPS[PIXMAP_TARGET] = new QPixmap(":/images/targettile.png");

    PIXMAPS[PIXMAP_SNOW] = new QPixmap(":/images/snowtile.png");
    PIXMAPS[PIXMAP_EX_SNOW] = new QPixmap(":/images/ex_snow.png");
    PIXMAPS[PIXMAP_LAVA] = new QPixmap(":/images/lavatile.png");

    PIXMAPS[PIXMAP_SNOWBALL_SMALL] = new QPixmap(":/images/snowball_small.png");
    PIXMAPS[PIXMAP_SNOWBALL_MEDIUM] = new QPixmap(":/images/snowball_medium.png");
    PIXMAPS[PIXMAP_SNOWBALL_BIG] = new QPixmap(":/images/snowball_big.png");

    PIXMAPS[PIXMAP_ICE] = new QPixmap(":/images/icetile.png");

}

QPixmap *Pixmap(PixmapIdentifier ident){
    return PIXMAPS[ident];
}


void *EnlargeMemory(int currentSize, int newSize, void *pointer){
    void *temp = malloc(newSize);
    memcpy(temp, pointer, currentSize);
    free(pointer);
    return temp;
}


char *DuplicateString(char *string)
{
    long long length = strlen(string);
    char *result = (char *)malloc(sizeof(char)*(length+1));
    memcpy(result, string, sizeof(char)*length);
    result[length] = 0;
    return result;
}

