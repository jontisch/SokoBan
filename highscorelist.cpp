#include "highscorelist.h"

HighscoreList::HighscoreList(Map *map)
{
    _map = map;
    _backgroundMap = new Map(MapFilename("highscore.fml"));
}

HighscoreList::~HighscoreList()
{
    delete _backgroundMap;
}

void HighscoreList::draw(QPainter *painter, QRect rect)
{
    _backgroundMap->draw(painter, rect);
    int tileSize = _backgroundMap->calculateTileSize(rect);
    QRect previewRect = _backgroundMap->tilesToRect(3, 3, 15, 11, rect);
    painter->save();
    painter->setClipRect(previewRect.marginsRemoved(QMargins(0, 0, tileSize*0.25, tileSize*0.25)));
    painter->fillRect(previewRect, Qt::black);
    _map->draw(painter, previewRect);
    painter->restore();
}

Map *HighscoreList::map()
{
    return _map;
}
