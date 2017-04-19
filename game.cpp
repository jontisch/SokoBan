#include "game.h"
#include <QDebug>
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
    if(_map->isSolved()){
        painter->save();
        painter->setOpacity(0.6);
        painter->fillRect(renderRect, Qt::black);
        painter->setOpacity(1);
        painter->setPen(Qt::white);
        painter->setFont(QFont("Arial", renderRect.width()*0.08273, 75));
        painter->drawText(QRect(renderRect.left(), renderRect.top()+renderRect.height()/2, renderRect.width(), renderRect.height()/2),
                          QString("Moves made: ") + QString::number(_map->movesMade()),
                          Qt::AlignHCenter | Qt::AlignTop);

        painter->drawText(QRect(renderRect.left(), renderRect.top(), renderRect.width(), renderRect.height()/2),
                          QString("Name:\n") + _playerName + "|",
                          Qt::AlignHCenter | Qt::AlignBottom);


        painter->restore();

    }

}


void Game::keyDown(Qt::Key key)
{
    if(!_map) return;

    if(!_map->isSolved()){
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
      default:
            break;
        }
    } else {
        QChar character((char)key);
        if((character.isLetterOrNumber() || character.isPunct() || character == ' ') && key != Qt::Key_Shift)
        {
            _playerName.append(character);
        }

        switch(key){
        case Qt::Key_Backspace:
            _playerName.truncate(_playerName.length()-1);
            break;
        case Qt::Key_Return:
            _map->insertHighscore(_playerName, _map->movesMade());
            _map->saveHighscores();
            if(_delegate == NULL)
                break;
            _delegate->mapCompleted(_map);
            break;
        default:
            break;
        }
    }

}

void Game::keyUp(Qt::Key key)
{

}

bool Game::hasMap()
{
    return(_map);
}

void Game::setMap(Map *map)
{
    if(_map) delete _map;
    _map = map;
}

void Game::setDelegate(GameDelegate *delegate)
{
    _delegate = delegate;
}

Game::~Game()
{
    delete _map;
}
