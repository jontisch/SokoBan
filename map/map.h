#ifndef MAP_H
#define MAP_H


#define MAX_MAP_SIZE 128

#include "../global.h"
#include "move_stack.h"
#include "tile.h"
#include "entities/entity.h"
#include "../collection.h"

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

    //If the tile already had an interactable, setTileInteractable returns the old one.
    Entity *setTileInteractable(int x, int y, Entity *interactable);
    bool tileHasInteractable(int x, int y, Entity *interactable);

    //These two are called by a ColoredEntity so that map can keep track of them (see ColoredEntity::setColor and ColoredEntity::Colored())
    void addColoredEntity(ColoredEntity *entity);
    void updateEntityColor(ColoredEntity *entity, EntityColor oldColor);

    //NOTE: The returned collection is the same as the internal one.
    Collection<Entity *> *entitiesByColor(EntityColor color);
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

    //Calculate the render rect for a tile
    QRect calculateTileRect(int x, int y, QPoint mapPixelOffset, int tileSize, int depth, int zOffset);

    //This sorts entities by colors
    Collection<Entity *> *_coloredEntities[N_ENTITY_COLORS];
};

#endif // MAP_H