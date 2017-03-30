#include "global.h"
#include "mainwindow.h"


QPixmap **PIXMAPS;


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
    long length = strlen(string);
    char *result = (char *)malloc(sizeof(char)*(length+1));
    memcpy(result, string, sizeof(char)*length);
    result[length] = 0;
    return result;
}

