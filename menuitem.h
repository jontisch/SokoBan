#ifndef MENUITEM_H
#define MENUITEM_H
#include <QString>
#include <QRect>
#include <QPainter>
#include "global.h"
#include "collection.h"
#include "game.h"

class Menu;

class MenuItem
{
public:
    MenuItem(QString text, Menu *ownerMenu, bool visible=true);
    QString getText();
    bool renderMenuItem(QRect renderRect, QPainter *qp, bool isCurrent);
    virtual Menu *select() = 0;
    Menu *getOwner();
    bool isVisible();
protected:
    QString _text;
    Menu *_ownerMenu;
private:
    bool _visible;
};


class Menu: public MenuItem
{
public:
    Menu(QString title, Menu *ownerMenu = NULL, QString text = NULL, bool visible=true);
    QString getTitle();
    void renderMenu(QRect renderRect, QPainter *qp);
    void addMenuItem(MenuItem *item);
    MenuItem *getSelected();
    void nextPos(bool forward);
    Menu *menuSelect();
    Menu *select();
protected:
    QString _title;
    Collection<MenuItem*> _menuItems;
private:
    int _currentPos;
};




class MenuAction: public MenuItem
{
public:
    MenuAction(QString text, Menu *ownerMenu, void(*action)(), bool visible=true);
    Menu *select();
protected:
    void(*_action)();
};

class LoadMapMenuAction: public MenuAction
{
public:
    LoadMapMenuAction(QString text, Menu *ownerMenu, Game *game, QString mapFilename, bool visible=true);
    Menu *select();

private:
    Game *_game;
    QString _mapFilename;
};




#endif // MENUITEM_H


