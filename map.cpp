#include "map.h"
#include <QDebug>

void Map::setup()
{
    _moveStack = new MoveStack();

}


Map::Map(int width, int height, int playerX, int playerY): _width(width), _height(height), _player(playerX,playerY), _movesMade(0)
{
    tiles = (Tiletype *)malloc(sizeof(Tiletype)* width * height);

    setup();
}

Map::Map(QString filename): _movesMade(0){
    QFile mapFile(filename);


    if(!mapFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "File not open";
    }

    QTextStream in(&mapFile);

    Tiletype *temporaryTiles = (Tiletype *)malloc(sizeof(Tiletype)*100*100);

    targetsLeft = 0;

    int y = 0;
    int c = 0;
    while(!in.atEnd()) {
        QString line = in.readLine();
        if(y == 0) _width = line.length();
        for(int x = 0; x < line.length(); x++)
        {
            const char character = line.at(x).toLatin1();
            int integer = character - 48;
            Tiletype type = (Tiletype)integer;
            temporaryTiles[c] = type;
            if(type == START){
                _player = QPoint(x,y);
            }
            if(type == TARGET)
            {
                targetsLeft++;
            }

            c++;
        }
        y++;
    }

    _height = y;
    tiles = (Tiletype *)malloc(sizeof(Tiletype)*_width*_height);
    memcpy(tiles, temporaryTiles, sizeof(Tiletype)*_width*_height);

    referenceTiles = (Tiletype *)malloc(sizeof(Tiletype)*_width*_height);
    memcpy(referenceTiles, temporaryTiles, sizeof(Tiletype)*_width*_height);
    for(int i = 0; i < _width * _height; i++)
    {
        if(referenceTiles[i] == BOX) referenceTiles[i] = FLOOR;
    }


    free(temporaryTiles);
    mapFile.close();

    setup();
}
void Map::setTile(int x, int y, Tiletype Type)
{
    if(getTile(x, y) == TARGET)
    {
        if(Type != TARGET) targetsLeft--;
    }
    else
    {
        if(Type == TARGET) targetsLeft++;
    }
    tiles[x+_width*y] = Type;
}

Tiletype Map::getTile(int x, int y)
{
    return tiles[ x + _width * y ];
}

int Map::calculateTileSize(QRect renderRect)
{
    int tileWidth = renderRect.width()/_width;
    int tileHeight = renderRect.height()/_height;
    int tileSize = (tileWidth > tileHeight) ? tileHeight : tileWidth;
    while(tileSize % 4 != 0) tileSize--;
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
            out << (char)((int)getTile(x,y) + 48);
        }
        out << "\n";
    }
    mapFile.close();
}

void Map::draw(QPainter *qp, QRect rect)
{
    if(_width <= 0 || _height <= 0) return;

    int tileSize = calculateTileSize(rect);
    QPoint pixelOffset = calculatePixelOffset(tileSize, rect);
    for(int x = 0; x < _width;x++){
        for(int y = 0; y < _height; y++){

            int size = 0;
            QBrush brush;
            QPixmap *pixmap = NULL;
            switch(tiles[x + _width * y])
            {
            case FLOOR:

                pixmap = Pixmap(FLOOR);
                break;
            case BOX:
                size = 4;
                pixmap = Pixmap(BOX);
                break;
            case WALL:
                size = 8;
                pixmap = Pixmap(WALL);
                break;
            case TARGET:
                pixmap = Pixmap(TARGET);
                break;
            case START:
                pixmap = Pixmap(START);
                break;
            case WATER:
                pixmap = Pixmap(WATER);
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
            if(x == _player.x() && y == _player.y()){
                qp->drawPixmap(pixelOffset.x() + x * tileSize,
                               pixelOffset.y() + y * tileSize,
                               tileSize, tileSize, QPixmap(":/images/playah.png"),0,0,32,32);
            }
        }
    }
}

void Map::movePlayer(int dx, int dy, bool force)
{
    if(force)
    {
        _player.setX(_player.x()+dx);
        _player.setY(_player.y()+dy);
        return;
    }

    //TODO @DebugStuff
    if(dx > 1 || dx < -1 || dy > 1 || dy < -1 || dx * dy != 0 ||
            (dx == 0 && dy == 0)) return;

    MoveStack::Move move = {0};

    int newX = _player.x()+dx;
    int newY = _player.y()+dy;
    int nextTile = newX + newY* _width;
    if(tiles[nextTile] != WALL){

        bool canWalk = true;

        if(tiles[nextTile] == BOX)
        {
            int boxX = newX;
            int boxY = newY;
            int boxNewX = boxX + dx;
            int boxNewY = boxY + dy;

            if(tiles[boxNewX+_width*boxNewY] == WALL ||
               tiles[boxNewX+_width*boxNewY] == BOX)
            {
                canWalk = false;
            }
            else
            {
                move.nChangedTiles = 2;
                move.changedTiles[0] = { boxNewX, boxNewY, tiles[boxNewX + _width * boxNewY] };
                move.changedTiles[1] = { boxX, boxY, tiles[boxX + _width * boxY] };

                setTile(boxNewX, boxNewY, BOX);
                setTile(boxX, boxY, referenceTiles[boxX + _width * boxY]);
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


void Map::revertMove()
{
    _moveStack->revertMove(this);
}

Map::~Map()
{
    free(tiles);
    free(referenceTiles);
    delete _moveStack;
}
