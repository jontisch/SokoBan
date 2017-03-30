#ifndef GAME_H
#define GAME_H
#include "qnamespace.h"
#include <QPainter>
#include "map.h"

class Game
{
public:
    Game();
    Game(QString mapFilename);

    void keyDown(Qt::Key key);
    void keyUp(Qt::Key key);

    bool hasMap();
    void draw(QPainter *painter, QRect renderRect);
    bool loadMap(QString mapFilename);
private:
    Map *_map;
};

#endif // GAME_H
