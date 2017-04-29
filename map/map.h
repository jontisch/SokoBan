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

    bool saveMap(QString mapName);
    bool loaded();
    QString filename();
    QString name();

    void draw(QPainter * qp, QRect rect);

    QPoint pixelToTile(int x, int y, QRect renderRect);
    QPoint tileToPixel(int x, int y, QRect renderRect);
    QRect tilesToRect(int x1, int y1, int x2, int y2, QRect renderRect);
    int calculateTileSize(QRect renderRect);
    QPoint calculatePixelOffset(int tileSize, QRect renderRect);

    int width();
    int height();
    void setSize(QSize size);
    bool tileInBounds(int x, int y);
    void shiftTiles(Direction dir);

    void setPlayerVisible(bool value);
    void movePlayer(int dx, int dy, bool force = false);
    void setPlayerPosition(int x, int y);
    void revertMove();

    void setTile(int x, int y, TileType Type);
    void fillType(int x, int y, TileType Type);
    TileType getTileType(int x, int y);
    void setTileFlags(int x, int y, int flags);
    void addTileFlag(int x, int y, TileFlag flag);
    void removeTileFlag(int x, int y, TileFlag flag);
    int tileFlags(int x, int y);

    void addEntity(int x, int y, EntityType entity, EntityColor color);
    void removeEntity(int x, int y);


    void addTileFlagToRect(QRect rect, TileFlag flag);
    void setTilesInRect(QRect rect, TileType type);
    void setTileFlagsInRect(QRect rect, int flags);
    void removeTileFlagFromRect(QRect rect, TileFlag flag);

    bool tileIsWalkable(int x, int y);

    //If the tile already had an interactable, setTileInteractable returns the old one.
    Entity *setTileInteractable(int x, int y, Entity *interactable);
    bool tileHasInteractable(int x, int y, Entity *interactable);

    //These two are called by a ColoredEntity so that map can keep track of them (see ColoredEntity::setColor and ColoredEntity::Colored())
    void addColoredEntity(Entity *entity);
    void updateEntityColor(Entity *entity, EntityColor oldColor);

    //NOTE: The returned collection is the same as the internal one.
    Collection<Entity *> *entitiesByColor(EntityColor color);
    int movesMade();

    void setName(QString name);

    void insertHighscore(QString playerName, int movesMade);
    bool saveHighscores();

    int nHighscores();
    bool highscoreRow(int index, QString *outputName, int *outputMoves, bool *outputLastInserted);

    bool isSolved();

protected:

private:
    void drawTilePixmap(QPainter *qp,
                        PixmapIdentifier pixmapIdentifier,
                        int x, int y,
                        QPoint pixelOffset,
                        int tileSize, int zOffset = 0,
                        PixmapIdentifier overlay = NO_PIXMAP,
                        PixmapIdentifier decoration = NO_PIXMAP);

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
    bool _solved;
    bool _loaded;
    QString _filename;
    QString _name;

    void setup();
    Tile *tile(int x, int y);

    //Calculate the render rect for a tile
    QRect calculateTileRect(int x, int y, QPoint mapPixelOffset, int tileSize, int depth, int zOffset);

    //This sorts entities by colors
    Collection<Entity *> *_coloredEntities[N_ENTITY_COLORS];

    struct {
        QString name;
        int moves;
    } _highscores[10];
    int _nHighscores;
    int _lastHighscoreIndex;
};

#endif // MAP_H
