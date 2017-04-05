#include "levelgrid.h"
#include <QDebug>

LevelGrid::LevelGrid(QString text, int width, int height)
{
    _width = width;
    _height = height;

    _currentX = 0;
    _currentY = 0;

    _nItems = 0;
    _items = (LevelGridItem **)malloc(sizeof(LevelGridItem *)*_width*_height);


    QString appPath = QCoreApplication::applicationDirPath() + MAP_DIR;
    _backgroundMap = new Map(QString(appPath + "/level_select.fml"));
    _backgroundMap->setPlayerVisible(false);
}

LevelGrid::~LevelGrid()
{
    delete _backgroundMap;
    for(int i = 0; i < _nItems; i++)
    {
        delete _items[i];
    }
    free(_items);
}

bool LevelGrid::addItem(LevelGridItem *item)
{
    if(_nItems >= _width * _height) return false;
    _items[_nItems] = item;
    _nItems++;
    return true;
}

void LevelGrid::draw(QPainter *painter, QRect renderRect)
{
    _backgroundMap->draw(painter, renderRect);
    int tileSize = _backgroundMap->calculateTileSize(renderRect);
    QPoint pixelOffset = _backgroundMap->calculatePixelOffset(tileSize, renderRect);
    QPoint topLeft(renderRect.x() + pixelOffset.x() + tileSize * 2.8,
                    renderRect.y() + pixelOffset.y() + tileSize * 9.85);

    for(int y = 0; y < _width; y++)
    {
        for(int x = 0; x < _height; x++)
        {
            int index = x + _width * y;
            if(index >= _nItems) break;
            QRect rect(topLeft.x() + tileSize * (5+11) * x,
                       topLeft.y() + tileSize * (5+7) * y,
                       tileSize*11, tileSize*7);
            _items[index]->drawPreview(painter, rect);
            _items[index]->drawText(painter, QRect(rect.x(), rect.y() + rect.height(), tileSize * 11, tileSize *1.8));
        }
    }
}

bool LevelGrid::move(Direction direction)
{
    int newCurrentX = XModifiedByDirection(_currentX, direction);
    int newCurrentY = XModifiedByDirection(_currentX, direction);
    if(newCurrentX < 0 || newCurrentX >= _width || newCurrentY < 0 || newCurrentY >= _height)
        return false;

    _currentX = newCurrentX;
    _currentY = newCurrentY;
    return true;
}

LevelGridItem::LevelGridItem(QString mapFilename, QString text)
{
    _mapFilename = mapFilename;
    _wasSelected = false;
    _text = text;

    _map = new Map(mapFilename);
}

Map *LevelGridItem::select()
{
    _wasSelected = true;
    return _map;
}

void LevelGridItem::drawPreview(QPainter *painter, QRect rect)
{
    _map->draw(painter, rect);
}

void LevelGridItem::drawText(QPainter *painter, QRect rect)
{
    painter->setFont(QFont("arial", 16, 10));
    painter->drawText(rect, Qt::AlignCenter, _text);
}

LevelGridItem::~LevelGridItem()
{
    if(!_wasSelected) delete _map;
}
