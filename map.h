#ifndef MAP_H
#define MAP_H


#define MAX_MAP_SIZE 128

#include "global.h"
#include "move_stack.h"
#include "tile.h"

#include <cstdlib>
#include <cstring>
#include <QPainter>
#include <QPoint>
#include <QFile>
#include <QMessageBox>
#include <QCoreApplication>

class MoveStack;

class Map
{
public:


    Map(int Width, int Height, int playerX, int playerY);
    Map(QString filename);
    ~Map();
    void setTile(int x, int y, TileType Type);
    TileType getTileType(int x, int y);
    void draw(QPainter * qp, QRect rect);
    void movePlayer(int dx, int dy, bool force = false);
    void setPlayerPosition(int x, int y);
    void revertMove();
    void addTileFlag(int x, int y, TileFlag flag);
    void removeTileFlag(int x, int y, TileFlag flag);
    QPoint pixelToTile(int x, int y, QRect renderRect);
    int calculateTileSize(QRect renderRect);
    QPoint calculatePixelOffset(int tileSize, QRect renderRect);
    void saveMap(QString mapName);
    bool loaded();
    int width();
    int height();
    void setSize(QSize size);
    void shiftTiles(Direction dir);
    void setPlayerVisible(bool value);
    void setTileFlags(int x, int y, int flags);
    bool tileInBounds(int x, int y);
    int tileFlags(int x, int y);

    bool tileIsWalkable(int x, int y);

protected:

private:
    void drawTilePixmap(QPainter *qp, PixmapIdentifier pixmapIdentifier, int x, int y, QPoint pixelOffset, int tileSize, int zOffset = 0, PixmapIdentifier overlay = NO_PIXMAP);

    bool tileHasMovable(int x, int y, TileFlag *outputMovable = NULL);
    bool tileIsEmptyOrItemCanBePushed(int x, int y, Direction direction, int itemsBetween = 0);
    void pushMovable(int x, int y, Direction dir, void *move);

    Tile *tiles;
    //TileType *referenceTiles;
    int _width;
    int _height;
    QPoint _startTile;
    bool _playerVisible;
    QPoint _player;
    MoveStack *_moveStack;
    int targetsLeft;
    int _movesMade;
    bool _loaded;


    void setup();
    Tile *tile(int x, int y);
};

#endif // MAP_H
