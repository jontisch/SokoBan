#ifndef HIGHSCORELIST_H
#define HIGHSCORELIST_H
#include <QPainter>
#include "map/map.h"

class HighscoreList
{
public:
    HighscoreList(Map *map);
    ~HighscoreList();
    void draw(QPainter *painter, QRect rect);
    Map *map();
private:
    Map *_map;
    Map *_backgroundMap;
};

#endif // HIGHSCORELIST_H
