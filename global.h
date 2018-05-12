#ifndef GLOBAL_H
#define GLOBAL_H

#include <QPixmap>
#include <QBrush>
#include <QCoreApplication>



#if (defined (_WIN32) || defined (_WIN64))
    #define APP_PATH_EXT ""
#else
    #define APP_PATH_EXT ""
#endif

#define MAP_DIR "maps/"
#define SOUND_DIR "sounds/"


#define Alloc(Type) ((Type *)malloc(sizeof(Type)))

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
    PIXMAP_PLAYER,
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
    PIXMAP_ICE,
    PIXMAP_GRASS,
    PIXMAP_SAND,
    PIXMAP_DOOR_VERTICAL,
    PIXMAP_DOOR_HORIZONTAL,
    PIXMAP_BUTTON_UP,
    PIXMAP_BUTTON_DOWN,
    PIXMAP_CACTUS,
    PIXMAP_MONSTER
};
#define N_PIXMAPS 22


enum SpriteIdentifier
{
    SPRITE_PLAYER,
    SPRITE_WATER,
    SPRITE_MONSTER,
    N_SPRITES
};

struct Sprite
{
    QPixmap **frames;
    int frameDuration;
    int nFrames;
    int zHeight;
};

Sprite *GetSprite(SpriteIdentifier identifier);
void InitSprites();



void InitPixmaps();

QPixmap *Pixmap(PixmapIdentifier index);

void *EnlargeMemory(int currentSize, int newSize, void *pointer);

char *DuplicateString(char *string);

QString LocalFilename(QString filename);
QString SoundFilename(QString filename);
QString MapFilename(QString filename);

#endif // GLOBAL_H
