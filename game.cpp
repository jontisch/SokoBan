#include "game.h"

Game::Game(): _map(NULL)
{

}

Game::Game(QString mapFilename) : Game()
{
    _map = new Map(mapFilename);
}

void Game::draw(QPainter *painter, QRect renderRect)
{
    if(!_map) return;
    _map->draw(painter, renderRect);
}


void Game::keyDown(Qt::Key key)
{
    if(!_map) return;
    switch(key){
    case Qt::Key_Up:
        _map->movePlayer(0,-1);
        break;
    case Qt::Key_Down:
        _map->movePlayer(0,1);
        break;
    case Qt::Key_Left:
        _map->movePlayer(-1,0);
        break;
    case Qt::Key_Right:
        _map->movePlayer(1,0);
        break;
    case Qt::Key_Z:
        _map->revertMove();
        break;
    case Qt::Key_Enter:
        _map->revertMove();
        break;
    default:
        break;
    }
}

void Game::keyUp(Qt::Key key)
{

}

bool Game::hasMap()
{
    return(_map);
}

bool Game::loadMap(QString mapFilename)
{
    if(_map) delete _map;
    _map = new Map(mapFilename);
    return _map->loaded();
}
