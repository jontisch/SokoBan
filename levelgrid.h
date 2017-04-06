#ifndef LEVELGRID_H
#define LEVELGRID_H

#include "menuitem.h"
#include <QPainter>

class LevelGrid;

class LevelGridItem
{
public:
    LevelGridItem(QString mapFilename, QString text);
    Map *select();
    void drawPreview(QPainter *painter, QRect rect);
    void drawText(QPainter *painter, QRect rect);
    ~LevelGridItem();
private:
    QString _mapFilename;
    QString _text;
    Map *_map;
};

class LevelGrid
{
public:
    LevelGrid(QString text, int width, int height);
    ~LevelGrid();

    //NOTE: When the MenuGrid is deconstructed, this item will be deleted.
    bool addItem(LevelGridItem *item);

    void draw(QPainter *painter, QRect renderRect);

    Map *selectCurrent();
    bool move(Direction direction);

private:
    void setCurrent(int x, int y);

    void setSelectionVisualsVisible(bool visible);

    int _currentX;
    int _currentY;
    int _width;
    int _height;
    int _nItems;
    LevelGridItem **_items;
    Map *_backgroundMap;
};

#endif // LEVELGRID_H
