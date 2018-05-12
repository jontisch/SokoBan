#include "global.h"
#include "mainwindow.h"
#include <QDebug>

QPixmap **PIXMAPS;
Sprite SPRITES[N_SPRITES];

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


void InitSprites()
{
    SPRITES[SPRITE_PLAYER].nFrames = 4;
    SPRITES[SPRITE_PLAYER].zHeight = 4;
    SPRITES[SPRITE_PLAYER].frameDuration = 10;
    SPRITES[SPRITE_PLAYER].frames = (QPixmap **)malloc(sizeof(QPixmap *)*SPRITES[SPRITE_PLAYER].nFrames);
    QPixmap sheet(":/images/player_spritesheet.png");
    for(int i = 0; i < SPRITES[SPRITE_PLAYER].nFrames; i++)
    {
        SPRITES[SPRITE_PLAYER].frames[i] = new QPixmap(sheet.copy(QRect(i*36, 0, 36, 36)));
    }


    SPRITES[SPRITE_WATER].nFrames = 4;
    SPRITES[SPRITE_WATER].zHeight = 0;
    SPRITES[SPRITE_WATER].frameDuration = 30;
    SPRITES[SPRITE_WATER].frames = (QPixmap **)malloc(sizeof(QPixmap *)*SPRITES[SPRITE_WATER].nFrames);
    QPixmap water_sheet(":/images/watertile_spritesheet.png");
    for(int i = 0; i < SPRITES[SPRITE_WATER].nFrames; i++)
    {
        SPRITES[SPRITE_WATER].frames[i] = new QPixmap(water_sheet.copy(QRect(i*32, 0, 32, 32)));
    }


    SPRITES[SPRITE_MONSTER].nFrames = 4;
    SPRITES[SPRITE_MONSTER].zHeight = 0;
    SPRITES[SPRITE_MONSTER].frameDuration = 5;
    SPRITES[SPRITE_MONSTER].frames = (QPixmap **)malloc(sizeof(QPixmap *)*SPRITES[SPRITE_MONSTER].nFrames);
    QPixmap monster_sheet(":/images/monster.png");
    for(int i = 0; i < SPRITES[SPRITE_MONSTER].nFrames; i++)
    {
        SPRITES[SPRITE_MONSTER].frames[i] = new QPixmap(monster_sheet.copy(QRect(i*36, 0, 36, 36)));
    }

}
Sprite *GetSprite(SpriteIdentifier identifier){
    return &SPRITES[identifier];
}

void InitPixmaps()
{
    PIXMAPS = (QPixmap **)malloc(sizeof(QPixmap *)*N_PIXMAPS);

    PIXMAPS[PIXMAP_PLAYER] = new QPixmap(":/images/player.png");

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

    PIXMAPS[PIXMAP_GRASS] = new QPixmap(":/images/grasstile.png");
    PIXMAPS[PIXMAP_SAND] = new QPixmap(":/images/sandtile.png");

    PIXMAPS[PIXMAP_DOOR_HORIZONTAL] = new QPixmap(":/images/door_closed_h.png");
    PIXMAPS[PIXMAP_DOOR_VERTICAL] = new QPixmap(":/images/door_closed_v.png");

    PIXMAPS[PIXMAP_BUTTON_UP] = new QPixmap(":/images/button_up.png");
    PIXMAPS[PIXMAP_BUTTON_DOWN] = new QPixmap(":/images/button_down.png");

    PIXMAPS[PIXMAP_CACTUS] = new QPixmap(":/images/cactus.png");

    PIXMAPS[PIXMAP_MONSTER] = GetSprite(SPRITE_MONSTER)->frames[0];

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

QString SoundFilename(QString filename)
{
    return LocalFilename(SOUND_DIR + filename);
}

QString MapFilename(QString filename)
{
    qDebug() << "map filename:" << MAP_DIR << filename;
    return LocalFilename(MAP_DIR + filename);
}

QString LocalFilename(QString filename)
{
    return QCoreApplication::applicationDirPath() + QString(APP_PATH_EXT) + "/" + filename ;
}
