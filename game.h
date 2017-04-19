#ifndef GAME_H
#define GAME_H
#include "qnamespace.h"
#include <QPainter>
#include "map/map.h"
#include "gamedelegate.h"

class Game
{
public:
    Game();
    Game(QString mapFilename);

    void keyDown(Qt::Key key);
    void keyUp(Qt::Key key);

    bool hasMap();
    void draw(QPainter *painter, QRect renderRect);
    void setMap(Map *map);
    void setDelegate(GameDelegate *delegate);

private:
    Map *_map;
    GameDelegate *_delegate;
    QString _playerName;

};

#endif // GAME_H
