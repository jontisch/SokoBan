#ifndef MENUITEM_H
#define MENUITEM_H
#include <QString>
#include <QRect>
#include <QPainter>
#include "global.h"
#include "collection.h"
#include "game.h"
#include "menuactiondelegate.h"
#include "map/tile.h"

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
    void setVisible(bool visible);
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
    void setPos(int pos);
    Map *getMap();
protected:
    QString _title;
    Collection<MenuItem*> _menuItems;
private:

    struct MenuRenderingMeasurements
    {
        int tileSize;
        int topPadding;
        int menuOffset;
        int itemHeight;
        int itemWidth;
        int spacing;
        int itemX;
        int headerFontSize;
        int itemFontSize;
    };

    QRect calculateItemRenderRect(int index, MenuRenderingMeasurements *m);
    Menu::MenuRenderingMeasurements calculateRenderingMeasurements(QRect renderRect);
    int _currentPos;
    Map *_bgMap;
    void updateMap();
};




class MenuAction: public MenuItem
{
public:
    MenuAction(QString text, Menu *ownerMenu, int action, MenuActionDelegate *delegate, bool visible=true);
    Menu *select();

protected:
    int _action;
private:
    MenuActionDelegate *_delegate;
};



#endif // MENUITEM_H


