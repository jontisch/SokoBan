#include "levelgrid.h"
#include <QDebug>

LevelGrid::LevelGrid(QString text, int width, int height): _text(text)
{
    _width = width;
    _height = height;

    _nItems = 0;
    _allocatedItems = _width * _height;
    _items = (LevelGridItem **)malloc(sizeof(LevelGridItem *)*_allocatedItems);


    _backgroundMap = new Map(QString(":/maps/level_select.fml"));
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
    if(_nItems >= _allocatedItems)
    {
        _allocatedItems *= 2;
        _items = (LevelGridItem **)EnlargeMemory(sizeof(LevelGridItem*)*_allocatedItems/2, sizeof(LevelGridItem*)*_allocatedItems, _items);
    }

    _items[_nItems] = item;
    _nItems++;
    return true;
}

void LevelGrid::draw(QPainter *painter, QRect renderRect)
{
    _backgroundMap->draw(painter, renderRect);
    int nPages = _nItems / (_width * _height)+1;
    int page = (_currentY * _width + _currentX)/ (_width * _height);

    for(int y = _height * page; y < _height * page + _height; y++)
    {
        for(int x = 0; x < _width; x++)
        {
            int pageX = x;
            int pageY = (page > 0) ? y % (page * _height) : y;

            int index = x + _width * y;
            if(index >= _nItems) break;
            int left = 3 + (5+11) * pageX;
            int top = 10 + (5+7) * pageY;
            int tileSize = _backgroundMap->calculateTileSize(renderRect);
            QRect rect = _backgroundMap->tilesToRect(left, top, left + 11, top + 7, renderRect);
            painter->save();
            painter->setClipRect(rect.marginsRemoved(QMargins(0, 0, tileSize*0.25, tileSize*0.25)));
            LevelGridItem *gridItem = _items[index];
            _items[index]->drawPreview(painter, rect);
            painter->restore();
            QRect textRect = _backgroundMap->tilesToRect(left, top + 7, left + 11, top + 7 + 2, renderRect);
            _items[index]->drawText(painter, textRect);
        }
    }
    QRect mapRect = _backgroundMap->tilesToRect(0,0,_backgroundMap->width(), _backgroundMap->height(), renderRect).adjusted(10, 0, -10, 0);

    QRect textRect = QRect(mapRect.x(), mapRect.bottom()-mapRect.height()*0.08, mapRect.width(), mapRect.height()*0.08);
    QRect headerRect = _backgroundMap->tilesToRect(0,0,_backgroundMap->width(), 7, renderRect);
    painter->setFont(QFont("Arial", _backgroundMap->calculateTileSize(renderRect) * 3, 75));
    painter->drawText(headerRect, _text, (QTextOption)Qt::AlignCenter);
    painter->setFont(QFont("Arial", mapRect.height()*0.05, 75));
    painter->drawText(textRect,
                      QString::number(page+1) + "/" + QString::number(nPages), (QTextOption)Qt::AlignCenter);
    painter->setFont(QFont("Arial", mapRect.height()*0.02, 75));
    painter->drawText(textRect.adjusted(0, mapRect.height()*0.01, 0, mapRect.height()*0.01), "X = High scores | C = Toggle custom maps", Qt::AlignVCenter | Qt::AlignRight);
}

Map *LevelGrid::selectCurrent()
{
    return _items[_currentX + _width * _currentY]->select();
}

bool LevelGrid::move(Direction direction)
{
    int newCurrentX = XModifiedByDirection(_currentX, direction);
    int newCurrentY = YModifiedByDirection(_currentY, direction);

    int rows = _nItems/_width;
    if(_nItems%_width) rows++;

    if(newCurrentX < 0 ||
            newCurrentY < 0 ||
            newCurrentX >= _width ||
            newCurrentY >= rows)

        return false;

    while(newCurrentX > 0 && (
              (_nItems % _width > 0 && newCurrentY == _nItems / _width && newCurrentX >= _nItems % _width) ||
               newCurrentY > _nItems / (_width+1)
          ))
        newCurrentX--;

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
    int page = (_currentY * _width + _currentX) / (_width * _height);
    int indexOnPage = (_currentY * _width + _currentX) % (_width * _height);
    int selectedY = indexOnPage / _width;
    int selectedX = indexOnPage % _width;

    for(int y = 0; y < _height; y++)
    {
        for(int x = 0; x < _width; x++)
        {
            if(x != selectedX || y != selectedY) continue;

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
