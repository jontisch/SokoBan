#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPaintEvent>
#include "map/map.h"
#include "menuitem.h"
#include "game.h"
#include "global.h"
#include "levelgrid.h"
#include "highscorelist.h"
#include "gamedelegate.h"
#include "editor/editor.h"
#include <QTimer>


enum AppState
{
    STATE_MENU,
    STATE_GAME,
    STATE_LEVEL_SELECT,
    STATE_HIGHSCORE,
    STATE_NEW_HIGHSCORE,
    STATE_EDITOR
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public MenuActionDelegate, public GameDelegate
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void executeMenuAction(int action);
    void mapCompleted(Map *map);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent * event);
    void keyPressEvent(QKeyEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

private slots:
    void timerTimeout();

private:
    void showHighscore(Map *map);
    void populateMaps();
    void populateCustomMaps();

    enum Action{
        RESUME_GAME,
        QUIT_GAME,
        PLAY_LEVEL
    };

    QTimer *timer;

    AppState _state;

    void initMenus();
    Game *_game;

    Menu *_mainMenu;
    Menu *_activeMenu;
    MenuItem *_resumeItem;

    HighscoreList *_highscores;

    LevelGrid *_standardLevelSelectGrid;
    LevelGrid *_customLevelSelectGrid;
    LevelGrid *_visibleLevelGrid;
    Editor *_editor;
};

#endif // MAINWINDOW_H
