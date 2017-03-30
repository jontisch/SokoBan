#include "map.h"
#include <QDebug>

void Map::setup()
{
    _moveStack = new MoveStack();
}


Map::Map(int width, int height, int playerX, int playerY):
    _width(width),
    _height(height),
    _player(playerX,playerY),
    _movesMade(0),
    _startTile(playerX, playerY),
    _loaded(true),
    _playerVisible(true)
{
    tiles = (Tile *)malloc(sizeof(Tile)* width * height);

    setup();
}

Map::Map(QString filename):
    _movesMade(0),
    _startTile(0, 0),
    _loaded(false),
    _playerVisible(true)
{
    QFile mapFile(filename);


    if(!mapFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "File not open";
    }

    QTextStream in(&mapFile);

    Tile *temporaryTiles = (Tile *)malloc(sizeof(Tile)*MAX_MAP_SIZE*MAX_MAP_SIZE);

    targetsLeft = 0;

    int y = 0;
    int c = 0;
    bool metaFound = false;
    while(!in.atEnd()) {
        QString line = in.readLine();
        if(y == 0) _width = line.length();
        if(line.length() < 1) break;
        if(line.at(0) == '#')
        {
            metaFound = true;
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

    /*
    referenceTiles = (TileType *)malloc(sizeof(TileType)*_width*_height);
    memcpy(referenceTiles, temporaryTiles, sizeof(TileType)*_width*_height);
    for(int i = 0; i < _width * _height; i++)
    {
        if(referenceTiles[i] == BOX) referenceTiles[i] = FLOOR;
    }
    */

    free(temporaryTiles);
    mapFile.close();

    setup();
    _loaded = true;
}
void Map::setTile(int x, int y, TileType Type)
{
    tiles[x+_width*y].type = Type;
}

TileType Map::getTileType(int x, int y)
{
    return tiles[ x + _width * y ].type;
}

int Map::calculateTileSize(QRect renderRect)
{
    int tileWidth = renderRect.width()/_width;
    int tileHeight = renderRect.height()/_height;
    int tileSize = (tileWidth > tileHeight) ? tileHeight : tileWidth;

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

    return QPoint(tileX, tileY);
}

void Map::saveMap(QString filename)
{
    //QString appPath = QCoreApplication::applicationDirPath();
    QFile mapFile(filename);
    if(!mapFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {

        //QMessageBox msgBox;
        //msgBox.setText("File doesn't exist");
        //msgBox.setInformativeText("Do you want to save your changes?");
    }
    QTextStream out(&mapFile);
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
    mapFile.close();
}

bool Map::loaded()
{
    return _loaded;
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

    Tile *tmp = (Tile*)malloc(sizeof(Tile)*w*h);
    memset(tmp, 0, sizeof(Tile)*w*h);

    Tile *current = tmp;
    int rowElements = _width;
    int rowsToCopy = _height;
    if(w < _width){
        rowElements = w;
    }
    if(h < _height){
        rowsToCopy = h;
    }

    for(int i = 0; i < rowsToCopy; i++){
        memcpy(current, &tiles[i*_width], sizeof(Tile)*rowElements);
        current += w;
    }
    _width = w;
    _height = h;

    free(tiles);
    tiles = tmp;
}

void Map::shiftTiles(Map::TileShiftDir dir)
{
    switch(dir){
    case UP:
    {
        Tile *firstRow = (Tile*)malloc(sizeof(Tile)*_width);
        memcpy(firstRow, tiles, sizeof(Tile)*_width);
        memcpy(tiles, &tiles[_width], sizeof(Tile)*_width*(_height-1));
        memcpy(&tiles[_width*(_height-1)], firstRow, sizeof(Tile)*_width);
        movePlayer(0,-1,true);
        break;
    }
    case DOWN:
    {
        Tile *lastRow = (Tile*)malloc(sizeof(Tile)*_width);
        memcpy(lastRow, &tiles[(_height-1)*_width], sizeof(Tile)*_width);
        memcpy(&tiles[_width], tiles, sizeof(Tile)*_width*(_height-1));
        memcpy(tiles, lastRow, sizeof(Tile)*_width);
        movePlayer(0,1,true);
        break;
    }
    case LEFT:
    {
        for(int i = 0; i < _height; i++){
            Tile first = tiles[i*_width];
            memcpy(&tiles[i*_width], &tiles[i*_width+1], sizeof(Tile)*(_width-1));
            tiles[i*_width+_width-1] = first;
        }
        movePlayer(-1,0,true);
        break;
    }
    case RIGHT:
    {
        for(int i = 0; i < _height; i++){
            Tile last = tiles[i*_width+_width-1];
            memcpy(&tiles[i*_width+1], &tiles[i*_width], sizeof(Tile)*(_width-1));
            tiles[i*_width] = last;
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

void Map::drawTilePixmap(QPainter *qp, PixmapIdentifier pixmapIdentifier, int x, int y, QPoint pixelOffset, int tileSize)
{
    int size = 0;
    QBrush brush;
    QPixmap *pixmap = Pixmap(pixmapIdentifier);
    switch(pixmapIdentifier)
    {
    case PIXMAP_BOX:
        size = 4;
        break;
    case PIXMAP_WALL:
        size = 8;
        break;
    default:
        break;
    }

    if(pixmap == NULL)
    {
        qp->fillRect(pixelOffset.x() + x * tileSize,
                     pixelOffset.y() + y * tileSize,
                     tileSize, tileSize, brush);
    }
    else
    {
        int depth = (size * tileSize / 32.0);
        qp->drawPixmap(pixelOffset.x() + x * tileSize - depth,
                       pixelOffset.y() + y * tileSize - depth,
                       tileSize + depth, tileSize + depth, *pixmap);
    }

}

void Map::draw(QPainter *qp, QRect rect)
{
    if(_width <= 0 || _height <= 0) return;

    int tileSize = calculateTileSize(rect);
    QPoint pixelOffset = calculatePixelOffset(tileSize, rect);
    for(int x = 0; x < _width;x++){
        for(int y = 0; y < _height; y++){

            if(tiles[x + _width * y].flags & HAS_BOX)
            {
                drawTilePixmap(qp, PIXMAP_BOX, x, y, pixelOffset, tileSize);
            }
            else
            {
                drawTilePixmap(qp, PixmapForTileType(tiles[x + _width * y].type), x, y, pixelOffset, tileSize);
                if(tiles[x + _width * y].flags & IS_TARGET)
                {
                    drawTilePixmap(qp, PIXMAP_TARGET, x, y, pixelOffset, tileSize);
                }
            }

            if(_playerVisible && x == _player.x() && y == _player.y()){
                qp->drawPixmap(pixelOffset.x() + x * tileSize,
                               pixelOffset.y() + y * tileSize,
                               tileSize, tileSize, QPixmap(":/images/playah.png"),0,0,32,32);
            }

        }
    }
}

void Map::addTileFlag(int x, int y, TileFlag flag)
{
    bool hadBox = tiles[x + _width * y].flags & HAS_BOX;

    if(flag == HAS_BOX && !hadBox && tiles[x + _width * y].flags & IS_TARGET)
    {
        targetsLeft--;
    }
    else if(flag == IS_START)
    {
        removeTileFlag(_startTile.x(), _startTile.y(), IS_START);
    }
    tiles[x + _width * y].flags |= flag;
}

void Map::removeTileFlag(int x, int y, TileFlag flag)
{
    bool hadBox = tiles[x + _width * y].flags & HAS_BOX;
    if(flag == HAS_BOX && hadBox && tiles[x + _width * y].flags & IS_TARGET)
    {
        targetsLeft++;
    }
    tiles[x + _width * y].flags &= ~(flag);
}

bool Map::tileIsWalkable(int x, int y)
{
    if(x < 0 || y < 0 || x >= _width || y >= _height) return false;
    TileType type = getTileType(x, y);
    return (type != WALL && type != WATER);
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
        _player.setX(newX);
        _player.setY(newY);
        return;
    }

    //TODO @DebugStuff
    if(dx > 1 || dx < -1 || dy > 1 || dy < -1 || dx * dy != 0 ||
            (dx == 0 && dy == 0)) return;

    MoveStack::Move move = {0};

    int newX = _player.x()+dx;
    int newY = _player.y()+dy;
    int nextTile = newX + newY* _width;
    if(tileIsWalkable(newX, newY)){

        bool canWalk = true;

        if(tiles[nextTile].flags & HAS_BOX)
        {
            int boxX = newX;
            int boxY = newY;
            int boxNewX = boxX + dx;
            int boxNewY = boxY + dy;

            if(!tileIsWalkable(boxNewX, boxNewY) ||
               tiles[boxNewX+_width*boxNewY].flags & HAS_BOX)
            {
                canWalk = false;
            }
            else
            {
                move.movedABox = true;
                move.movedBox.fromX = boxX;
                move.movedBox.fromY = boxY;
                move.movedBox.toX = boxNewX;
                move.movedBox.toY = boxNewY;

                addTileFlag(boxNewX,boxNewY, HAS_BOX);
                removeTileFlag(boxX, boxY, HAS_BOX);
            }
        }

        if(canWalk)
        {
            _player.setX(_player.x()+dx);
            _player.setY(_player.y()+dy);
            move.playerDX = dx;
            move.playerDY = dy;
            _moveStack->pushMove(move);
            _movesMade++;
        }
    }

    if(targetsLeft <= 0)
    {
        qDebug() << "Victory!" << _movesMade << " moves made";
    }
}

void Map::setPlayerPosition(int x, int y)
{
    _player = QPoint(x, y);
}


void Map::revertMove()
{
    _moveStack->revertMove(this);
}

Map::~Map()
{
    free(tiles);
    //free(referenceTiles);
    delete _moveStack;
}
