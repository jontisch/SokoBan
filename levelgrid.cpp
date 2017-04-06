#include "levelgrid.h"
#include <QDebug>

LevelGrid::LevelGrid(QString text, int width, int height)
{
    _width = width;
    _height = height;

    _nItems = 0;
    _items = (LevelGridItem **)malloc(sizeof(LevelGridItem *)*_width*_height);


    _backgroundMap = new Map(MapFilename("level_select.fml"));
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

    for(int y = 0; y < _height; y++)
    {
        for(int x = 0; x < _width; x++)
        {
            int index = x + _width * y;
            if(index >= _nItems) break;
            int left = 3 + (5+11) * x;
            int top = 10 + (5+7) * y;
            int tileSize = _backgroundMap->calculateTileSize(renderRect);
            QRect rect = _backgroundMap->tilesToRect(left, top, left + 11, top + 7, renderRect);
            painter->save();
            painter->setClipRect(rect.marginsRemoved(QMargins(0, 0, tileSize*0.25, tileSize*0.25)));
            _items[index]->drawPreview(painter, rect);
            painter->restore();
            QRect textRect = _backgroundMap->tilesToRect(left, top + 7, left + 11, top + 7 + 2, renderRect);
            _items[index]->drawText(painter, textRect);
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
    if(newCurrentX < 0 ||
            newCurrentY < 0 ||
            newCurrentX >= _width ||
            newCurrentY >= _height ||

            (_nItems % _width > 0 && newCurrentY == _nItems / _width && newCurrentX >= _nItems % _width) ||
            newCurrentY > _nItems / (_width+1))

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
    for(int y = 0; y < _height; y++)
    {
        for(int x = 0; x < _width; x++)
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

LevelGridItem::LevelGridItem(QString mapFilename)
{
    _mapFilename = mapFilename;
    _map = new Map(mapFilename);
    if(_map->loaded())
    {
        _text = _map->name();
    }
    else _text = "";
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
