
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <dirent.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _mapNames(32)
{
    ui->setupUi(this);

    _menuVisible = true;
    InitPixmaps();
    this->setFixedSize(1024,768);

    _game = new Game();
    this->initMenus();

}

void MainWindow::executeMenuAction(int action)
{
    if(_activeMenu == _mainMenu){
        switch(action){
        case RESUME_GAME:
            _menuVisible = false;
            _activeMenu = _mainMenu;
            break;
        case QUIT_GAME:
            QApplication::quit();
            break;
        default:
            break;
        }
    }
    else if(_activeMenu == _playMenu)
    {
        QString appPath = QCoreApplication::applicationDirPath() + MAP_DIR;
        char *mapName;
        if(_mapNames.get(action, &mapName))
        {
            if(_game->loadMap(appPath + "/" + mapName))
            {
                _menuVisible = false;
                _activeMenu = _mainMenu;
            }
        }
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::keyPressEvent(QKeyEvent *event)
{

    if(!_menuVisible && event->key() == Qt::Key_Escape){

        _resumeItem->setVisible(_game->hasMap());
        _menuVisible = true;
        _activeMenu = _mainMenu;
        _mainMenu->setPos(0);

        this->repaint();
        return;
    }

    if(_menuVisible)
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
            if(_activeMenu == NULL) _menuVisible = false;
            break;
        }
        case Qt::Key_Escape:

            Menu *owner = _activeMenu->getOwner();
            if(owner != NULL || _game->hasMap())
            {
                _menuVisible = (owner != NULL);
                _activeMenu = owner;
            }

            break;
        }
    }
    else
    {
        _game->keyDown((Qt::Key)event->key());
    }

    this->repaint();
}

void MainWindow::initMenus()
{
    _mainMenu = new Menu("Main Menu");

    _resumeItem = new MenuAction("Resume", _mainMenu, RESUME_GAME, this, false);

    _mainMenu->addMenuItem(_resumeItem);

    _playMenu = new Menu("Play Game", _mainMenu, "Play game");
    _mainMenu->addMenuItem(_playMenu);

    MenuAction *exitMenu = new MenuAction("Exit", _mainMenu, QUIT_GAME, this, true);
    _mainMenu->addMenuItem(exitMenu);

    DIR *dir;
    struct dirent *ent;
    QString appPath = QCoreApplication::applicationDirPath() + MAP_DIR;
    if ((dir = opendir (appPath.toStdString().c_str())) != NULL){
        int counter = 0;
        while ((ent = readdir(dir)) != NULL){
            if(ent->d_name[0] != '.'){
                char *curMap = DuplicateString(ent->d_name);
                _mapNames.add(curMap);
                _playMenu->addMenuItem(new MenuAction(QString(curMap), _playMenu, counter, this, true));
                counter++;
            }

        }
        closedir(dir);
    }else{
        qDebug() << "Couldn't read path" << appPath.toStdString().c_str();
    }
    _activeMenu = _mainMenu;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter qp(this);

    if(_game->hasMap()){
        _game->draw(&qp, this->rect());
    }
    if(_menuVisible){
        _activeMenu->renderMenu(this->rect(),&qp);
    }
}
