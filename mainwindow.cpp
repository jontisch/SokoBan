
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDesktopWidget>
#include <dirent.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _mapNames(32)
{
    ui->setupUi(this);

    _state = STATE_MENU;

    InitPixmaps();
    //QRect screenRect = QApplication::desktop()->screenGeometry();
    //this->setGeometry(screenRect.width()/2 - 1024/2, screenRect.height()/2-768/2, 1024,768);

    _game = new Game();
    this->initMenus();

    _levelSelectGrid = new LevelGrid("grid", 3, 2);
    QString appPath = QCoreApplication::applicationDirPath();
    LevelGridItem *potato_power = new LevelGridItem(appPath + MAP_DIR + "/potato_power.fml", "Potato Power");
    LevelGridItem *feel_rect = new LevelGridItem(appPath + MAP_DIR + "/feel_rect.fml", "Feel Rect");
    LevelGridItem *simplex = new LevelGridItem(appPath + MAP_DIR + "/map.fml", "Simplex");
    LevelGridItem *frozen_island = new LevelGridItem(appPath + MAP_DIR + "/frozen_island.fml", "Frozen Island");

    _levelSelectGrid->addItem(potato_power);
    _levelSelectGrid->addItem(feel_rect);
    _levelSelectGrid->addItem(simplex);
    _levelSelectGrid->addItem(frozen_island);

}

void MainWindow::executeMenuAction(int action)
{
    if(_activeMenu == _mainMenu){
        switch(action){
        case RESUME_GAME:
            _state = STATE_GAME;
            _activeMenu = _mainMenu;
            break;
        case PLAY_LEVEL:
            _state = STATE_LEVEL_SELECT;
            break;
        case QUIT_GAME:
            QApplication::quit();
            break;
        default:
            break;
        }
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::keyPressEvent(QKeyEvent *event)
{

    if(_state == STATE_GAME && event->key() == Qt::Key_Escape){

        _resumeItem->setVisible(_game->hasMap());
        _state = STATE_MENU;
        _activeMenu = _mainMenu;
        _mainMenu->setPos(0);

        this->repaint();
        return;
    }

    if(_state == STATE_MENU)
    {
        switch(event->key()){
        case Qt::Key_Up:
            _activeMenu->nextPos(false);
            break;
        case Qt::Key_Down:
            _activeMenu->nextPos(true);
            break;
        case Qt::Key_Left:
            _activeMenu->nextPos(false);
            break;
        case Qt::Key_Right:
            _activeMenu->nextPos(true);
            break;
        case Qt::Key_Return:
        {
            _activeMenu = _activeMenu->menuSelect();
            if(_activeMenu == NULL) _state = STATE_GAME;
            break;
        }
        case Qt::Key_Escape:

            Menu *owner = _activeMenu->getOwner();
            if(owner != NULL || _game->hasMap())
            {
                _state = (owner != NULL) ? STATE_MENU : STATE_GAME;
                _activeMenu = owner;
            }

            break;
        }
    }
    else if(_state == STATE_GAME)
    {
        _game->keyDown((Qt::Key)event->key());
    }
    else if(_state == STATE_LEVEL_SELECT)
    {
        switch(event->key()){
        case Qt::Key_Up:
            _levelSelectGrid->move(UP);
            break;
        case Qt::Key_Down:
            _levelSelectGrid->move(DOWN);
            break;
        case Qt::Key_Left:
            _levelSelectGrid->move(LEFT);
            break;
        case Qt::Key_Right:
            _levelSelectGrid->move(RIGHT);
            break;
        case Qt::Key_Return:
        {
            Map *levelMap = _levelSelectGrid->selectCurrent();
            if(levelMap)
            {
                QString mapFilename = levelMap->filename();
                _game->setMap(new Map(mapFilename));
                _state = STATE_GAME;
                qDebug() << _game->hasMap();
            }
            break;
        }
        case Qt::Key_Escape:
            _state = STATE_MENU;
            break;
        }
    }

    this->repaint();
}

void MainWindow::initMenus()
{
    _mainMenu = new Menu("Main Menu");

    _resumeItem = new MenuAction("Resume", _mainMenu, RESUME_GAME, this, false);

    _mainMenu->addMenuItem(_resumeItem);

    MenuAction *play = new MenuAction("Levels", _mainMenu, PLAY_LEVEL, this, true);
    _mainMenu->addMenuItem(play);

    MenuAction *exitMenu = new MenuAction("Exit", _mainMenu, QUIT_GAME, this, true);
    _mainMenu->addMenuItem(exitMenu);

    _activeMenu = _mainMenu;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter qp(this);
    qp.setOpacity(1.0);

    if(_state == STATE_GAME && _game->hasMap()){
        _game->draw(&qp, this->rect());
    }
    else if(_state == STATE_MENU){
        _activeMenu->renderMenu(this->rect(),&qp);
    }
    else if(_state == STATE_LEVEL_SELECT)
    {
        _levelSelectGrid->draw(&qp, this->rect());
    }
}
