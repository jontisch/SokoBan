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

enum AppState
{
    STATE_MENU,
    STATE_GAME,
    STATE_LEVEL_SELECT,
    STATE_HIGHSCORE
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public MenuActionDelegate
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void executeMenuAction(int action);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent * event);

    void keyPressEvent(QKeyEvent *event);
private:
    enum Action{
        RESUME_GAME,
        QUIT_GAME,
        PLAY_LEVEL
    };

    AppState _state;

    void initMenus();
    Game *_game;
    Ui::MainWindow *ui;

    Menu *_mainMenu;
    Menu *_activeMenu;
    MenuItem *_resumeItem;

    HighscoreList *_highscores;

    LevelGrid *_levelSelectGrid;
};

#endif // MAINWINDOW_H
