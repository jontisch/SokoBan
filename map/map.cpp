#include "map.h"
#include "entities/button.h"
#include <QDebug>
#include "entities/door.h"
#include <stdlib.h>
#include "../audiolibrary.h"
#include <thread>
#include <math.h>


using namespace std;

void Map::setup()
{
    _moveStack = new MoveStack();


    for(int i = 0; i < N_ENTITY_COLORS; i++)
    {
        _coloredEntities[i] = new Collection<Entity *>(8);
    }


}


Map::Map(int width, int height, int playerX, int playerY):
    _width(width),
    _height(height),
    _player(playerX,playerY),
    _movesMade(0),
    _startTile(playerX, playerY),
    _loaded(true),
    _playerVisible(true),
    _solved(false),
    _lastHighscoreIndex(-1),
    tempTiles(NULL),
    _playerTickIndex(0)

{
    tiles = (Tile *)malloc(sizeof(Tile)* width * height);
    memset(tiles, 0, sizeof(Tile)* width * height);

    setup();
}

Map::Map(QString filename):
    _movesMade(0),
    _startTile(0, 0),
    _loaded(false),
    _playerVisible(true),
    _filename(filename),
    _solved(false),
    _lastHighscoreIndex(-1),
    tempTiles(NULL),
    _playerTickIndex(0)
{
    setup();

    QFile mapFile(filename);

    if(!mapFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "File" << filename << "not open";
    }

    QTextStream in(&mapFile);

    Tile *temporaryTiles = (Tile *)malloc(sizeof(Tile)*MAX_MAP_SIZE*MAX_MAP_SIZE);
    memset(temporaryTiles, 0, sizeof(Tile)*MAX_MAP_SIZE*MAX_MAP_SIZE);

    targetsLeft = 0;

    if(in.atEnd())
    {
        qDebug() << "No map name found";
        mapFile.close();
        return;
    }

    QString nameLine = in.readLine();
    bool a = nameLine.length() >= 1;
    bool b = nameLine.at(0) == '$';
    if(a && b)
    {
        _name = nameLine.mid(1, nameLine.length()-1);
    }
    else
    {
        _name = "no name";
        in.seek(0);
    }

    int y = 0;
    int c = 0;
    bool metaFound = false;
    bool entitiesFound = false;
    while(!in.atEnd()) {
        QString line = in.readLine();
        if(y == 0) _width = line.length();
        if(line.length() < 1) break;
        if(line.at(0) == '#')
        {
            metaFound = true;
            break;
        }
        if(line.at(0) == '&')
        {
            entitiesFound = true;
            break;
        }
        for(int x = 0; x < line.length(); x++)
        {
            const char character = line.at(x).toLatin1();
            int integer = character - 48;

            TileType type = (TileType)integer;
            if(!IsValidTileType(type)) type = FLOOR;
            temporaryTiles[c].flags = NO_TILE_FLAGS;
            temporaryTiles[c].type = type;

            c++;
        }
        y++;
    }
    if(metaFound)
    {
        while(!in.atEnd())
        {
            QString line = in.readLine();
            if(line.at(0) == '&')
            {
                entitiesFound = true;
                break;
            }
            QStringList xSplit = line.split('x');
            QString xString = xSplit.at(0);
            if(xSplit.count() < 2)
            {
                qDebug() << "Invalid map meta data";
                break;
            }
            QStringList colonSplit = xSplit.at(1).split(':');
            QString yString = colonSplit.at(0);
            if(colonSplit.count() < 2)
            {
                qDebug() << "Invalid map meta data";
                break;
            }
            QString flagsString = colonSplit.at(1);

            //TODO Not safe...
            int x = atoi(xString.toLatin1().data());
            int y = atoi(yString.toLatin1().data());
            long flags = atol(flagsString.toLatin1().data());


            temporaryTiles[x + _width * y].flags = flags;
            if(flags & IS_TARGET)
            {
                targetsLeft++;
            }
            if(flags & IS_START)
            {
                _player = QPoint(x,y);
                _startTile = QPoint(x, y);
            }
        }
    }
    _height = y;
    tiles = (Tile *)malloc(sizeof(Tile)*_width*_height);
    memcpy(tiles, temporaryTiles, sizeof(Tile)*_width*_height);


    if(entitiesFound)
    {
        while(!in.atEnd())
        {
            QString line = in.readLine();
            QStringList xSplit = line.split('x');
            QString xString = xSplit.at(0);
            if(xSplit.count() < 2)
            {
                qDebug() << "Invalid entity meta data";
                break;
            }
            QStringList colonSplit = xSplit.at(1).split(':');
            QString yString = colonSplit.at(0);
            if(colonSplit.count() < 2)
            {
                qDebug() << "Invalid entity meta data";
                break;
            }
            QString entityString = colonSplit.at(1);

            //TODO Not safe...
            int x = atoi(xString.toLatin1().data());
            int y = atoi(yString.toLatin1().data());

            QStringList commaSplit = entityString.split(',');
            if(commaSplit.count() < 2)
            {
                qDebug() << "Invalid entity meta data";
                break;
            }
            else
            {
                EntityType type = (EntityType)atoi(commaSplit.at(0).toLatin1().data());
                EntityColor color = (EntityColor)atoi(commaSplit.at(1).toLatin1().data());
                addEntity(x,y,type,color);
            }


        }
    }


    QFile highscoreFile(filename + ".hs");

    _nHighscores = 0;
    if(highscoreFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream hin(&highscoreFile);
        while(!hin.atEnd())
        {
            QString nameLine = hin.readLine();

            if(nameLine.length() < 1) break;
            if(hin.atEnd()) break;
            QString movesLine = hin.readLine();
            if(movesLine.length() < 1) break;
            _highscores[_nHighscores].name = nameLine;
            bool movesParsed = false;
            _highscores[_nHighscores].moves = movesLine.toInt(&movesParsed);
            if(movesParsed) _nHighscores++;
        }
    }

    free(temporaryTiles);
    mapFile.close();

    _loaded = true;


    tile(0,0)->decoration = CACTUS;
}
void Map::setTile(int x, int y, TileType Type)
{
    currentTileArray()[x+_width*y].type = Type;
}

void Map::fillType(int x, int y, TileType Type)
{
    Tile *currentTiles = currentTileArray();
    TileType currentType = currentTiles[x+_width*y].type;
    if(currentTiles[x+_width*y].type == Type) return;
    currentTiles[x+_width*y].type = Type;

    if(x > 0){
        if(currentTiles[x-1+_width*y].type == currentType){
            fillType(x-1, y, Type);
        }
    }
    if(x < _width){
        if(currentTiles[x+1+_width*y].type == currentType){
            fillType(x+1, y, Type);
        }
    }
    if(y > 0){
        if(currentTiles[x+_width*(y-1)].type == currentType){
            fillType(x, y-1, Type);
        }
    }
    if(y < _height){
        if(currentTiles[x+_width*(y+1)].type == currentType){
            fillType(x, y+1, Type);
        }
    }
}
TileType Map::getTileType(int x, int y)
{
    return tiles[ x + _width * y ].type;
}

void Map::addTileFlagToRect(QRect rect, TileFlag flag)
{
    if(!tileInBounds(rect.x(), rect.y()) || !tileInBounds(rect.x() + rect.width() - 1, rect.y() + rect.height()-1))
        return;

    for(int x = rect.x(); x < rect.x() + rect.width(); x++)
    {
        for(int y = rect.y(); y < rect.y() + rect.height(); y++)
        {
            addTileFlag(x, y, flag);
        }
    }
}

void Map::removeTileFlagFromRect(QRect rect, TileFlag flag)
{
    for(int x = rect.x(); x < rect.x() + rect.width(); x++)
    {
        for(int y = rect.y(); y < rect.y() + rect.height(); y++)
        {
            removeTileFlag(x, y, flag);
        }
    }
}


void Map::setTileFlagsInRect(QRect rect, int flags)
{
    for(int x = rect.x(); x < rect.width(); x++)
    {
        for(int y = rect.y(); y < rect.height(); y++)
        {
            setTileFlags(x, y, flags);
        }
    }
}

void Map::setTilesInRect(QRect rect, TileType type)
{   
    int x1, x2, y1, y2;
    rect.getCoords(&x1, &y1, &x2, &y2);
    rect = QRect(QPoint(min(x1, x2), min(y1, y2)), QPoint(max(x1, x2), max(y1, y2)));

    if(rect.left() < 0) rect.setLeft(0);
    if(rect.top()) rect.setTop(0);
    if(rect.right() >= _width) rect.setRight(_width-1);
    if(rect.bottom() >= _height) rect.setBottom(_height-1);

    for(int y = rect.top(); y <= rect.bottom(); y++)
    {
        for(int x = rect.left(); x <= rect.right(); x++)
        {
            setTile(x, y, type);
        }
    }
}

int Map::calculateTileSize(QRect renderRect)
{
    int tileWidth = renderRect.width()/_width;
    int tileHeight = renderRect.height()/_height;
    int tileSize = (tileWidth > tileHeight) ? tileHeight : tileWidth;

    while(tileSize % 2 != 0) tileSize--;

    return tileSize;
}

QPoint Map::calculatePixelOffset(int tileSize, QRect renderRect)
{
    int xOffset = (renderRect.width() - tileSize*_width)/2;
    int yOffset = (renderRect.height() - tileSize*_height)/2;
    return QPoint(xOffset, yOffset);
}

QPoint Map::pixelToTile(int x, int y, QRect renderRect)
{
    int tileSize = calculateTileSize(renderRect);
    QPoint pixelOffset = calculatePixelOffset(tileSize, renderRect);

    int minX = renderRect.x() + pixelOffset.x();
    int minY = renderRect.y() + pixelOffset.y();

    int tileX = (x - minX)/tileSize;
    int tileY = (y - minY)/tileSize;

    // if(!tileInBounds(tileX, tileY)) return QPoint(-1, -1);

    return QPoint(tileX, tileY);
}

QPoint Map::tileToPixel(int x, int y, QRect renderRect)
{
    int tileSize = calculateTileSize(renderRect);
    QPoint pixelOffset = calculatePixelOffset(tileSize, renderRect);
    return QPoint(x * tileSize + pixelOffset.x(),
                  y * tileSize + pixelOffset.y());
}

QRect Map::tilesToRect(int x1, int y1, int x2, int y2, QRect renderRect)
{
    int tileSize = calculateTileSize(renderRect);
    QPoint pixelOffset = calculatePixelOffset(tileSize, renderRect);
    return QRect(x1 * tileSize + pixelOffset.x(),
                 y1 * tileSize + pixelOffset.y(),
                 tileSize * (x2-x1),
                 tileSize * (y2-y1));
}

bool Map::saveMap(QString filename)
{
    QFile mapFile(filename);
    if(!mapFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return false;
    }
    QTextStream out(&mapFile);

    out << '$' << _name << "\n";

    for(int y = 0; y < _height; y++){
        for(int x = 0; x < _width; x++){
            out << (char)((int)getTileType(x,y) + 48);
        }
        out << "\n";
    }
    out << "#\n";
    for(int y = 0; y < _height; y++){
        for(int x = 0; x < _width; x++){
            long flags = tiles[x + _width * y].flags;
            if(flags != NO_TILE_FLAGS)
            {
                out << x << "x" << y << ":" << flags << "\n";
            }
        }
    }

    out << "&\n";
    for(int y = 0; y < _height; y++){
        for(int x = 0; x < _width; x++){

            Tile *t = tile(x,y);

            if(t->interactable != NULL){
                out << x << "x" << y << ":";
                out << t->interactable->getEntityType() << "," << t->interactable->color() << "\n";
            }

        }
    }
    mapFile.close();

    _filename = filename;

    return true;
}

bool Map::loaded()
{
    return _loaded;
}

QString Map::filename()
{
    return _filename;
}

QString Map::name()
{
    return _name;
}

int Map::width()
{
    return _width;
}

int Map::height()
{
    return _height;
}

void Map::setSize(QSize size)
{
    int w = size.width();
    int h = size.height();

    tiles = resizeTileArray(w, h, _width, _height, tiles);
    if(tempTiles)
        tempTiles = resizeTileArray(w, h, _width, _height, tempTiles);
    _width = w;
    _height = h;
}

Tile* Map::resizeTileArray(int w, int h, int currentWidth, int currentHeight, Tile* source){
    Tile *tmp = (Tile*)malloc(sizeof(Tile)*w*h);
    memset(tmp, 0, sizeof(Tile)*w*h);

    Tile *current = tmp;
    int rowElements = currentWidth;
    int rowsToCopy = currentHeight;
    if(w < currentWidth){
        rowElements = w;
    }
    if(h < currentHeight){
        rowsToCopy = h;
    }

    for(int i = 0; i < rowsToCopy; i++){
        memcpy(current, &source[i*currentWidth], sizeof(Tile)*rowElements);
        current += w;
    }
    free(source);
    return tmp;
}

Tile *Map::currentTileArray()
{
    return(tempTiles)?tempTiles:tiles;
}

void Map::shiftTiles(Direction dir)
{
    Tile *currentTiles = currentTileArray();
    switch(dir){
    case UP:
    {
        Tile *firstRow = (Tile*)malloc(sizeof(Tile)*_width);
        memcpy(firstRow, currentTiles, sizeof(Tile)*_width);
        memcpy(currentTiles, &currentTiles[_width], sizeof(Tile)*_width*(_height-1));
        memcpy(&currentTiles[_width*(_height-1)], firstRow, sizeof(Tile)*_width);
        movePlayer(0,-1,true);
        break;
    }
    case DOWN:
    {
        Tile *lastRow = (Tile*)malloc(sizeof(Tile)*_width);
        memcpy(lastRow, &currentTiles[(_height-1)*_width], sizeof(Tile)*_width);
        memcpy(&currentTiles[_width], currentTiles, sizeof(Tile)*_width*(_height-1));
        memcpy(currentTiles, lastRow, sizeof(Tile)*_width);
        movePlayer(0,1,true);
        break;
    }
    case LEFT:
    {
        for(int i = 0; i < _height; i++){
            Tile first = currentTiles[i*_width];
            memcpy(&currentTiles[i*_width], &currentTiles[i*_width+1], sizeof(Tile)*(_width-1));
            currentTiles[i*_width+_width-1] = first;
        }
        movePlayer(-1,0,true);
        break;
    }
    case RIGHT:
    {
        for(int i = 0; i < _height; i++){
            Tile last = currentTiles[i*_width+_width-1];
            memcpy(&currentTiles[i*_width+1], &currentTiles[i*_width], sizeof(Tile)*(_width-1));
            currentTiles[i*_width] = last;
        }
        movePlayer(1,0,true);
        break;
    }
    default:
        break;

    }
}

void Map::setPlayerVisible(bool value)
{
    _playerVisible = value;
}

QRect Map::calculateTileRect(int x, int y, QPoint mapPixelOffset, int tileSize, int depth, int zOffset)
{
    int rectX = mapPixelOffset.x() + x * tileSize - zOffset - depth;
    int rectY = mapPixelOffset.y() + y * tileSize - zOffset - depth;
    int rectWidth = tileSize + depth;
    int rectHeight = tileSize + depth;
    return QRect(rectX, rectY, rectWidth, rectHeight);
}

void Map::drawTilePixmap(QPainter *qp,
                         PixmapIdentifier pixmapIdentifier,
                         int x, int y, QPoint topLeft, int tileSize, int zOffset,
                         PixmapIdentifier overlay,
                         PixmapIdentifier decoration)
{
    int size = 0;
    QBrush brush;
    QPixmap *pixmap = Pixmap(pixmapIdentifier);
    switch(pixmapIdentifier)
    {
    case PIXMAP_PLAYER:
    case PIXMAP_BOX:
        size = 4;
        break;
    case PIXMAP_WALL:
        size = 8;
        break;
    case PIXMAP_CACTUS:
        size = 8;
        break;
    default:
        break;
    }

    int depth = (size * tileSize / 32.0);
    QRect renderRect = calculateTileRect(x, y, topLeft, tileSize, depth, zOffset);

    if(pixmapIdentifier != NO_PIXMAP)
    {
        if(pixmap == NULL)
        {
            qp->fillRect(renderRect, brush);
        }
        else
        {
            qp->drawPixmap(renderRect, *pixmap);
        }
    }

    if(overlay != NO_PIXMAP || decoration != NO_PIXMAP)
    {
        drawTilePixmap(qp, overlay, x, y, topLeft, tileSize, depth, decoration);
    }

}

void Map::drawTileSprite(QPainter *qp, SpriteIdentifier spriteIdentifier, int x, int y, QPoint pixelOffset, int tileSize, int *tickIndex)
{
    Sprite *sprite = GetSprite(spriteIdentifier);
    int frameIndex = (*tickIndex/sprite->frameDuration) % sprite->nFrames;
    QPixmap frame = *sprite->frames[frameIndex];


    int depth = (sprite->zHeight/32.0f)*tileSize;
    qp->drawPixmap(QRect(pixelOffset.x() + x * tileSize - depth,
                         pixelOffset.y() + y * tileSize - depth,
                         tileSize + depth,
                         tileSize + depth), frame);
    (*tickIndex)++;
}

void Map::draw(QPainter *qp, QRect rect)
{
    if(_width <= 0 || _height <= 0) return;


    int tileSize = calculateTileSize(rect);
    QPoint pixelOffset = calculatePixelOffset(tileSize, rect);

    QPoint topLeft(rect.x() + pixelOffset.x(), rect.y() + pixelOffset.y());

    Tile *toDraw = currentTileArray();

    for(int x = 0; x < _width;x++){
        for(int y = 0; y < _height; y++){

            Tile *tile = &toDraw[x + _width * y];

            if(tile->flags & HAS_BOX)
            {
                drawTilePixmap(qp, PIXMAP_BOX, x, y, topLeft, tileSize);
            }
            else
            {
                PixmapIdentifier overlay = NO_PIXMAP;
                PixmapIdentifier decoration = NO_PIXMAP;

                if(tile->type != WATER && tile->flags & HAS_SNOW)
                {
                    overlay = PIXMAP_SNOW;
                }
                else if(tile->flags & WAS_SNOW) overlay = PIXMAP_EX_SNOW;

                decoration = PixmapForDecoration(tile->decoration);

                if(tile->type != WATER)
                {
                    PixmapIdentifier tilePixmap = PixmapForTileType(tile->type);
                    if(tile->type == WATER && tile->flags & HAS_SNOW)
                    {
                        tilePixmap = PIXMAP_ICE;
                    }
                    drawTilePixmap(qp, tilePixmap, x, y, topLeft, tileSize, 0, overlay, decoration);
                }
                else
                {
                    drawTileSprite(qp, SPRITE_WATER, x, y, topLeft, tileSize, &tile->tick);
                }

                if(tile->flags & IS_TARGET)
                {
                    drawTilePixmap(qp, PIXMAP_TARGET, x, y, topLeft, tileSize);
                }
                if(tile->interactable != NULL)
                {
                    tile->interactable->drawAt(qp, calculateTileRect(x, y, topLeft, tileSize, tile->interactable->height()*tileSize, 0));
                }
                if(tile->flags & HAS_SNOWBALL_SMALL)
                {
                    drawTilePixmap(qp, PIXMAP_SNOWBALL_SMALL, x, y, topLeft, tileSize, 0);
                }
                else if(tile->flags & HAS_SNOWBALL_MEDIUM)
                {
                    drawTilePixmap(qp, PIXMAP_SNOWBALL_MEDIUM, x, y, topLeft, tileSize, 0);
                }
                else if(tile->flags & HAS_SNOWBALL_BIG)
                {
                    drawTilePixmap(qp, PIXMAP_SNOWBALL_BIG, x, y, topLeft, tileSize, 0);
                }


            }

            if(_playerVisible && x == _player.x() && y == _player.y()){
                drawTileSprite(qp, SPRITE_PLAYER, x, y, topLeft, tileSize, &_playerTickIndex);
                //playerFrameIndex++;

                //if(playerFrameIndex >= 4) playerFrameIndex = 0;

                //drawTilePixmap(qp, PIXMAP_PLAYER, x, y, topLeft, tileSize, 0);
            }

        }
    }
}

void Map::addTileFlag(int x, int y, TileFlag flag)
{
    setTileFlags(x, y, tileFlags(x,y) | flag);
}

void Map::removeTileFlag(int x, int y, TileFlag flag)
{
    setTileFlags(x, y, tileFlags(x,y) & ~(flag));
}

void Map::setTileFlags(int x, int y, int flags)
{
    Tile *t = tile(x,y);
    if(t == NULL) return;
    bool hadMovable = tileHasMovable(x, y);
    bool wasTarget = t->flags & IS_TARGET;
    t->flags = flags;
    bool hasMovable = tileHasMovable(x,y);
    bool isTarget = t->flags & IS_TARGET;
    if(wasTarget && (!isTarget || (!hadMovable && hasMovable)))
    {
        targetsLeft--;
    }
    else if((!wasTarget && isTarget && !hasMovable) || (isTarget && !hasMovable && hadMovable))
    {
        targetsLeft++;
    }

    if(t->interactable)
    {
        //Tell the interactable that a movable has entered or exited
        if(!hadMovable && hasMovable)
        {
            t->interactable->movableEntered(x, y);
        }
        else if(hadMovable && !hasMovable)
        {
            t->interactable->movableExited(x, y);
        }
    }


}

bool Map::tileIsWalkable(int x, int y)
{
    Tile *tile = this->tile(x, y);
    if(tile == NULL) return false;
    if(tile->interactable != NULL && tile->interactable->blocksPlayer()) return false;
    TileType type = getTileType(x, y);
    return (type != WALL && type != WATER);
}

bool Map::tileInBounds(int x, int y)
{
    return x >= 0 && y >= 0 && x < _width && y < _height;
}

bool Map::tileHasMovable(int x, int y, TileFlag *outputMovable)
{
    if(!tileInBounds(x,y)) return false;

    TileFlag fake;
    if(!outputMovable) outputMovable = &fake;

    Tile *t = &tiles[x + _width * y];
    if(t->flags & HAS_BOX){ *outputMovable = HAS_BOX; return true; }
    if(t->flags & HAS_SNOWBALL_SMALL){ *outputMovable = HAS_SNOWBALL_SMALL; return true; }
    if(t->flags & HAS_SNOWBALL_MEDIUM){ *outputMovable = HAS_SNOWBALL_MEDIUM; return true; }
    if(t->flags & HAS_SNOWBALL_BIG){ *outputMovable = HAS_SNOWBALL_BIG; return true; }
    return false;
}

bool Map::tileIsEmptyOrItemCanBePushed(int x, int y, Direction direction, int itemsBetween)
{
    if(!tileHasMovable(x, y)) return true;

    Tile *t = &tiles[x + _width * y];

    int nextX = XModifiedByDirection(x, direction);
    int nextY = YModifiedByDirection(y, direction);

    if(t->flags & HAS_BOX) return (itemsBetween < 1) && tileIsEmptyOrItemCanBePushed(nextX, nextY, direction, itemsBetween+1);
    if(t->flags & HAS_SNOWBALL_SMALL || t->flags & HAS_SNOWBALL_MEDIUM)
    {
        return tileIsEmptyOrItemCanBePushed(nextX, nextY, direction, itemsBetween+1);
    }
    if(t->flags & HAS_SNOWBALL_BIG)
    {
        return false;
    }

    return true;
}

Tile *Map::tile(int x, int y, bool useCurrent)
{
    if(!tileInBounds(x,y)) return NULL;
    return &((useCurrent)?currentTileArray():tiles)[x + _width * y];
}

int Map::tileFlags(int x, int y, bool useCurrent)
{
    if(!tileInBounds(x,y)) return 0;
    return tile(x,y,useCurrent)->flags;
}

//ENTITIES


void Map::addEntity(int x, int y, EntityType type, EntityColor color)
{
    Entity *ent = entityFromEntityType(type, color, this);

    setTileInteractable(x,y,ent);
}

void Map::removeEntity(int x, int y)
{
    setTileInteractable(x, y, NULL);
}

//ENTITIES

void Map::pushMovable(int x, int y, Direction dir, void *move)
{
    auto theMove = (MoveStack::Move *)move;

    int nextX = XModifiedByDirection(x, dir);
    int nextY = YModifiedByDirection(y, dir);

    TileFlag movable;
    if(!tileHasMovable(x, y, &movable)) return;

    MoveStack::addTileChange(theMove, x, y, tileFlags(x, y));

    bool flagAddedToNext = false;
    if(!tileInBounds(nextX, nextY))
    {
        removeTileFlag(x, y, movable);
    }
    else
    {
        if(tileHasMovable(nextX, nextY))
        {
            pushMovable(nextX, nextY, dir, move);
        }
        else
        {
            MoveStack::addTileChange(theMove, nextX, nextY, tileFlags(nextX, nextY));
        }

        TileFlag newMovable = movable;
        if(tileFlags(x, y) & HAS_SNOW && (movable == HAS_SNOWBALL_SMALL || movable == HAS_SNOWBALL_MEDIUM || movable == HAS_SNOWBALL_BIG))
        {
            if(movable == HAS_SNOWBALL_SMALL) newMovable = HAS_SNOWBALL_MEDIUM;
            else if(movable == HAS_SNOWBALL_MEDIUM) newMovable = HAS_SNOWBALL_BIG;
            removeTileFlag(x, y, HAS_SNOW);
            addTileFlag(x, y, WAS_SNOW);
        }
        addTileFlag(nextX, nextY, newMovable);
        removeTileFlag(x, y, movable);

        //(movable == HAS_BOX)
            //AudioLibrary::Instance()->playSound(SOUND_PUSH_BOX);
    }
}

void Map::movePlayer(int dx, int dy, bool force)
{
    if(force)
    {
        int newX = _player.x()+dx;
        int newY = _player.y()+dy;
        if(newX >= _width){
            newX = 0;
        }else if(newX < 0){
            newX = _width-1;
        }
        if(newY >= _height){
            newY = 0;
        }else if(newY < 0){
            newY = _height-1;
        }

        setPlayerPosition(newX, newY);
        return;
    }

    //TODO @DebugStuff
    if(dx > 1 || dx < -1 || dy > 1 || dy < -1 || dx * dy != 0 ||
            (dx == 0 && dy == 0)) return;

    Direction dir;
    if(dx < 0) dir = LEFT;
    else if(dx > 0) dir = RIGHT;
    else if(dy < 0) dir = UP;
    else if(dy > 0) dir = DOWN;

    MoveStack::Move move = {0};

    int newX = _player.x()+dx;
    int newY = _player.y()+dy;
    if(tileIsWalkable(newX, newY)){

        bool canWalk = true;

        TileFlag movable;
        if(tileHasMovable(newX, newY, &movable))
        {
            int movableX = newX;
            int movableY = newY;
            int movableNewX = movableX + dx;
            int movableNewY = movableY + dy;

            TileFlag tmp;
            if(!tileIsWalkable(movableNewX, movableNewY) ||
               !tileIsEmptyOrItemCanBePushed(movableX, movableY, dir))
            {
                canWalk = false;
            }
            else
            {
                pushMovable(movableX, movableY, dir, &move);
            }
        }

        if(canWalk)
        {
            setPlayerPosition(newX, newY);
            move.playerDX = dx;
            move.playerDY = dy;
            _moveStack->pushMove(move);
            _movesMade++;
        }
    }
    _solved = targetsLeft <= 0;

}

bool Map::tileHasInteractable(int x, int y, Entity *interactable)
{
    Tile *tile = this->tile(x, y);
    if(tile == NULL) return false;
    if(tile->interactable != NULL)
    {
        interactable = tile->interactable;
        return true;
    }
    return false;
}

void Map::addColoredEntity(Entity *entity)
{
    entitiesByColor(entity->color())->add(entity);
}

//Move the entity to the right collection when it changes color.
void Map::updateEntityColor(Entity *entity, EntityColor oldColor)
{
    Collection<Entity *> *oldCollection = entitiesByColor(oldColor);
    int index = oldCollection->indexOf(entity);
    if(index != -1)
        oldCollection->remove(index);
    entitiesByColor(entity->color())->add(entity);
}

Collection<Entity *> *Map::entitiesByColor(EntityColor color)
{
    return _coloredEntities[color-1];
}

int Map::movesMade()
{
    return _movesMade;
}

void Map::setName(QString name)
{
    _name = name;
}

void Map::insertHighscore(QString playerName, int movesMade)
{
    _lastHighscoreIndex = -1;
    bool inserted = false;
    for(int i = 0; i < _nHighscores; i++){

        if(_highscores[i].moves > movesMade){
            if(_nHighscores < 10)
                _nHighscores++;
            for(int j = _nHighscores-1; j > i; j--){
                if(j >= 9)
                    continue;
                _highscores[j] = _highscores[j-1];
            }
            _highscores[i].name = playerName.toUtf8().data();
            _highscores[i].moves = movesMade;
            inserted = true;
            _lastHighscoreIndex = i;
            break;
        }

    }
    if(!inserted && _nHighscores < 10){
        _highscores[_nHighscores].name = playerName.toLatin1().data();
        _highscores[_nHighscores].moves = movesMade;
        _lastHighscoreIndex = _nHighscores;
        _nHighscores++;
    }
}

bool Map::saveHighscores()
{

    QFile highscoreFile(_filename + ".hs");
    if(!highscoreFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return false;
    }

    QTextStream out(&highscoreFile);

    for(int i = 0; i < _nHighscores; i++)
    {
        auto row = _highscores[i];
        out << row.name << "\n";
        out << row.moves << "\n";
    }

    highscoreFile.close();

    return true;
}

int Map::nHighscores()
{
    return _nHighscores;
}

bool Map::highscoreRow(int index, QString *outputName, int *outputMoves, bool *outputLastInserted)
{
    if(index < 0 || index >= _nHighscores) return false;
    *outputName = _highscores[index].name;
    *outputMoves = _highscores[index].moves;
    *outputLastInserted = index == _lastHighscoreIndex;
    return true;
}

bool Map::isSolved()
{
    return _solved;
}

bool Map::isEditing()
{
    return(tempTiles);
}

void Map::beginEditing()
{
    if(tempTiles) return;

    tempTiles = (Tile*)malloc(sizeof(Tile)*_width*_height);
    memcpy(tempTiles, tiles, sizeof(Tile)*_width*_height);

}

void Map::applyChanges()
{
    if(tempTiles){
        memcpy(tiles, tempTiles, sizeof(Tile)*_width*_height);
        free(tempTiles);
        tempTiles = NULL;
    }
}

void Map::revertChanges()
{
    if(tempTiles){
        free(tempTiles);
        tempTiles = NULL;
    }
}

Entity *Map::setTileInteractable(int x, int y, Entity *interactable)
{
    Tile *tile = this->tile(x, y);
    if(tile == NULL) return NULL;
    Entity *result = tile->interactable;
    tile->interactable = interactable;
    return result;
}

void Map::setPlayerPosition(int x, int y)
{
    Tile *currentTile = tile(_player.x(), _player.y());
    Tile *newTile = tile(x, y);

    //Tell the interactables that the player has exited/entered their tiles.
    if(currentTile != NULL && currentTile->interactable != NULL)
            currentTile->interactable->playerExited(_player.x(), _player.y());
    if(newTile != NULL && newTile->interactable != NULL)
            newTile->interactable->playerEntered(x, y);

    _player.setX(x);
    _player.setY(y);
}


void Map::revertMove()
{
    _moveStack->revertMove(this);
}

Map::~Map()
{
    free(tiles);
    if(tempTiles)free(tempTiles);
    delete _moveStack;

    for(int i = 0; i < N_ENTITY_COLORS; i++)
    {
        delete _coloredEntities[i];
    }
}
