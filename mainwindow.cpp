
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    _state = STATE_MENU;

    InitSprites();
    InitPixmaps();

    setMouseTracking(true);

    _editor = new Editor();


    QRect screenRect = QApplication::desktop()->screenGeometry();
    this->setGeometry(screenRect.width()/2 - 1280/2, screenRect.height()/2-800/2, 1280,800);
    this->setMinimumSize(1280, 800);
    _game = new Game();
    _game->setDelegate(this);
    this->initMenus();

    _standardLevelSelectGrid = new LevelGrid("Standard maps", 3, 2);

    _customLevelSelectGrid = new LevelGrid("Custom maps", 3, 2);
    _visibleLevelGrid = _standardLevelSelectGrid;
    populateMaps();
    populateCustomMaps();

    timer = new QTimer(this);
    timer->setInterval(33);
    timer->start();
    connect(timer, SIGNAL(timeout()), this, SLOT(timerTimeout()));
    _highscores = NULL;
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

void MainWindow::mapCompleted(Map *map)
{
    showHighscore(map);
}


MainWindow::~MainWindow()
{
    delete _game;
    delete _editor;
    delete _standardLevelSelectGrid;
    delete _customLevelSelectGrid;
    if(_highscores){
        delete _highscores;
    }
}
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_F11){

        this->setWindowState(windowState() ^ Qt::WindowFullScreen);
    }
    if(_state == STATE_GAME && event->key() == Qt::Key_Escape){

        _resumeItem->setVisible(_game->hasMap());
        _state = STATE_MENU;
        _activeMenu = _mainMenu;
        _mainMenu->setPos(0);

        //this->repaint();
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
        } break;
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
            _visibleLevelGrid->move(UP);
            break;
        case Qt::Key_Down:
            _visibleLevelGrid->move(DOWN);
            break;
        case Qt::Key_Left:
            _visibleLevelGrid->move(LEFT);
            break;
        case Qt::Key_Right:
            _visibleLevelGrid->move(RIGHT);
            break;
        case Qt::Key_Return:
            {
                Map *levelMap = _visibleLevelGrid->selectCurrent();
                if(levelMap)
                {
                    QString mapFilename = levelMap->filename();
                    _game->setMap(new Map(mapFilename));
                    _state = STATE_GAME;
                }
            }
            break;
        case Qt::Key_X:
        {
            Map *levelMap = _visibleLevelGrid->selectCurrent();
            showHighscore(levelMap);
        } break;
        case Qt::Key_C:
        {
            _visibleLevelGrid = (_visibleLevelGrid == _customLevelSelectGrid)? _standardLevelSelectGrid:_customLevelSelectGrid;
        } break;
        case Qt::Key_Escape:
            _state = STATE_MENU;
            break;
        }
    }
    else if(_state == STATE_HIGHSCORE)
    {
        switch(event->key())
        {
            case Qt::Key_Escape:
                if(_highscores) delete _highscores;
                _highscores = NULL;
                _state = STATE_LEVEL_SELECT;
                break;
        }
    }
    else if(_state == STATE_EDITOR)
    {
        _editor->keyPress(event);
    }

    //this->repaint();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(_state == STATE_EDITOR){
        _editor->mouseRelease(event);
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(_state == STATE_EDITOR) _editor->mouseMove(event, this->rect().adjusted(0,32,0,0));
    //repaint();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(_state == STATE_EDITOR)
    {
        if(QRect(0, 0, 72, 32).contains(event->pos()))
        {
            _state = STATE_MENU;
        }
        else _editor->mousePress(event, this->rect().adjusted(0,32,0,0), this);
    }
    else if(_state == STATE_MENU && _activeMenu->getMap()->tilesToRect(0, 0, 0, 0, rect()).adjusted(0, 0, 72, 32).contains(event->pos()))
    {
        _state = STATE_EDITOR;
    }
    //repaint();
}

void MainWindow::timerTimeout()
{
    repaint();
}


void MainWindow::showHighscore(Map *map)
{
    if(map)
    {
        bool createNewHighscoreList = !_highscores || _highscores->map() != map;
        if(_highscores && createNewHighscoreList)
        {
            delete _highscores;
        }
        else
        {
            _highscores = new HighscoreList(map);
        }
        _state = STATE_HIGHSCORE;
    }
}
void MainWindow::populateMaps()
{
    QDir dir(LocalFilename("maps"));

    QStringList maps = dir.entryList(QStringList("*.fml"), (QDir::Filters)QDir::NoFilter, (QDir::SortFlags)QDir::Name);
    for(int i = 0; i < maps.length(); i++){
        _standardLevelSelectGrid->addItem(new LevelGridItem(dir.absolutePath() + "/" + maps.at(i)));
    }
}

void MainWindow::populateCustomMaps()
{
    QDir dir(LocalFilename("custom_maps"));

    QStringList maps = dir.entryList(QStringList("*.fml"), (QDir::Filters)QDir::NoFilter, (QDir::SortFlags)QDir::Time);
    for(int i = 0; i < maps.length(); i++){
        _customLevelSelectGrid->addItem(new LevelGridItem(dir.absolutePath() + "/" + maps.at(i)));
    }
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
    qp.fillRect(this->rect(), Qt::black);

    if(_state == STATE_GAME && _game->hasMap()){
        _game->draw(&qp, this->rect());
    }
    else if(_state == STATE_MENU){
        _activeMenu->renderMenu(this->rect(),&qp);
        qp.save();
        qp.setPen(Qt::white);
        qp.setBrush(Qt::black);
        qp.drawRoundedRect(_activeMenu->getMap()->tilesToRect(0, 0, 0, 0, rect()).adjusted(0, 0, 72, 32), 10,10);
        qp.setFont(QFont("Arial", 16, 50));
        qp.drawText(_activeMenu->getMap()->tilesToRect(0, 0, 0, 0, rect()).adjusted(0, 0, 72, 32), "Editor", (QTextOption)Qt::AlignCenter);
        qp.restore();
    }
    else if(_state == STATE_LEVEL_SELECT)
    {
        _visibleLevelGrid->draw(&qp, this->rect());
    }
    else if(_state == STATE_HIGHSCORE)
    {
        _highscores->draw(&qp, this->rect());
    }else if(_state == STATE_NEW_HIGHSCORE){
        _game->draw(&qp, this->rect());

    } else if(_state == STATE_EDITOR)
    {
        QRect editorRect = this->rect().adjusted(0, 32, 0, 0);
        _editor->draw(&qp, editorRect);
        qp.save();
        qp.setPen(Qt::white);
        qp.setBrush(Qt::black);
        qp.drawRoundedRect(QRect(0,0,72,32), 10,10);
        qp.setFont(QFont("Arial", 16, 50));
        qp.drawText(QRect(0, 0, 72, 32), "Exit", (QTextOption)Qt::AlignCenter);
        qp.restore();
    }
}
