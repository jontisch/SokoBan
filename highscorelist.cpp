#include "highscorelist.h"
#include <QDebug>

HighscoreList::HighscoreList(Map *map)
{
    _map = map;
    _backgroundMap = new Map(QString(":/maps/highscore.fml"));
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
    QRect titleRect = _backgroundMap->tilesToRect(3,11,15,13, rect);
    painter->save();
    painter->setClipRect(previewRect.marginsRemoved(QMargins(0, 0, tileSize*0.25, tileSize*0.25)));
    painter->fillRect(previewRect, Qt::black);
    _map->draw(painter, previewRect);
    painter->restore();
    painter->setFont(QFont("Arial", titleRect.height()*0.4, 50));
    painter->drawText(titleRect, _map->name(), (QTextOption)Qt::AlignCenter);

    QFont nameFont = QFont("arial", tileSize * 1.2, 75);
    QFont movesFont = QFont("arial", tileSize * 1.3, 100);

    QString name;
    int moves;
    int i = 0;
    bool last;
    while(_map->highscoreRow(i, &name, &moves, &last))
    {
        QRect nameRect = _backgroundMap->tilesToRect(19, 3+3*i, 19+17, 3+3*i+2, rect);
        nameRect.adjust(0, -0.175 * tileSize, 0, -0.175 * tileSize);
        QRect movesRect = _backgroundMap->tilesToRect(19+18+1, 3+3*i, 19+18+1+8, 3+3*i+2, rect);
        movesRect.adjust(0, -0.175 * tileSize, 0, -0.175 * tileSize);

        painter->setFont(nameFont);
        painter->setPen(last?Qt::red:Qt::black);
        painter->drawText(nameRect, name, Qt::AlignLeft | Qt::AlignVCenter);

        painter->setFont(movesFont);
        painter->setPen(last?Qt::red:Qt::white);
        painter->drawText(movesRect, QString::number(moves), Qt::AlignHCenter | Qt::AlignVCenter);
        i++;
    }
}

Map *HighscoreList::map()
{
    return _map;
}
