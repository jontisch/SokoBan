#include "levelgrid.h"
#include <QDebug>

LevelGrid::LevelGrid(QString text, int width, int height)
{
    _width = width;
    _height = height;

    _nItems = 0;
    _items = (LevelGridItem **)malloc(sizeof(LevelGridItem *)*_width*_height);


    QString appPath = QCoreApplication::applicationDirPath() + MAP_DIR;
    _backgroundMap = new Map(QString(appPath + "/level_select.fml"));
    _backgroundMap->setPlayerVisible(false);

    setCurrent(0, 0);
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
                       tileSize * 11, tileSize * 7);
            _items[index]->drawPreview(painter, rect);
            _items[index]->drawText(painter, QRect(rect.x(), rect.y() + rect.height(), tileSize * 11, tileSize *1.8));
        }
    }
}

Map *LevelGrid::selectCurrent()
{
    return _items[_currentX + _width * _currentY]->select();
}

bool LevelGrid::move(Direction direction)
{
    int newCurrentX = XModifiedByDirection(_currentX, direction);
    int newCurrentY = YModifiedByDirection(_currentY, direction);
    if(newCurrentX < 0 || newCurrentX >= _nItems % _width || newCurrentY < 0 || newCurrentY > _nItems/_width)
        return false;

    setCurrent(newCurrentX, newCurrentY);
    return true;
}


void LevelGrid::setCurrent(int x, int y)
{
    setSelectionVisualsVisible(false);
    _currentX = x;
    _currentY = y;
    setSelectionVisualsVisible(true);
}

void LevelGrid::setSelectionVisualsVisible(bool visible)
{
    for(int y = 0; y < _width; y++)
    {
        for(int x = 0; x < _height; x++)
        {
            if(x != _currentX || y != _currentY) continue;

            QRect rect(2 + (3+13) * x,
                       9 + (3+9) * y,
                       13, 10);
            TileFlag flag = HAS_SNOW;
            if(visible)
            {
                _backgroundMap->addTileFlagToRect(rect, flag);
                _backgroundMap->removeTileFlagFromRect(rect.marginsRemoved(QMargins(1,1,1,2)), flag);
            }
            else
                _backgroundMap->removeTileFlagFromRect(rect, flag);
        }
    }
}

LevelGridItem::LevelGridItem(QString mapFilename, QString text)
{
    _mapFilename = mapFilename;
    _text = text;

    _map = new Map(mapFilename);
}

Map *LevelGridItem::select()
{
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
    delete _map;
}
