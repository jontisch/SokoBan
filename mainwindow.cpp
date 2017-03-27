
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <dirent.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _menuVisible = true;
    initPixmaps();
    this->setFixedSize(1024,768);

    _game = new Game();
    this->initMenus();

}


MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::keyPressEvent(QKeyEvent *event)
{

    if(!_menuVisible && event->key() == Qt::Key_Escape){
        _menuVisible = true;
        _activeMenu = _mainMenu;
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
            if(owner != NULL || _game != NULL)
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

    _mainMenu->addMenuItem(new MenuAction("Resume", _mainMenu,[&]{}, true));

    Menu *playMenu = new Menu("Play Game", _mainMenu, "Play game");
    _mainMenu->addMenuItem(playMenu);

    MenuAction *exitMenu = new MenuAction("Exit", _mainMenu, &QApplication::quit);
    _mainMenu->addMenuItem(exitMenu);

    DIR *dir;
    struct dirent *ent;
    QString appPath = QCoreApplication::applicationDirPath() + MAP_DIR;
    if ((dir = opendir (appPath.toStdString().c_str())) != NULL){
        int counter = 0;
        while ((ent = readdir(dir)) != NULL){
            if(ent->d_name[0] != '.'){

                playMenu->addMenuItem(new LoadMapMenuAction(QString(ent->d_name), playMenu, _game, appPath + "/" + QString(ent->d_name)));
            }
            counter++;
        }
        closedir(dir);
    }else{
        qDebug() << "Couldn't read path";
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
