#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPaintEvent>
#include "map.h"
#include "menuitem.h"
#include "game.h"
#include "global.h"

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
    enum ACTION_TYPES{
        RESUME_GAME,
        QUIT_GAME
    };

    void initMenus();
    Game *_game;
    Ui::MainWindow *ui;
    Collection<char*> _mapNames;
    Menu *_mainMenu;
    Menu *_activeMenu;
    MenuItem *_resumeItem;
    Menu *_playMenu;
    bool _menuVisible;
};

#endif // MAINWINDOW_H
