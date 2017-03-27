#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPaintEvent>
#include "map.h"
#include "menuitem.h"
#include "game.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent * event);

    void keyPressEvent(QKeyEvent *event);
private:
    void initMenus();
    Game *_game;
    Ui::MainWindow *ui;
    Menu *_mainMenu;
    Menu *_activeMenu;
    bool _menuVisible;
};

#endif // MAINWINDOW_H
