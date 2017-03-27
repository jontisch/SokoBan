#include "menuitem.h"
#include <QDebug>
#include <assert.h>

MenuItem::MenuItem(QString text, Menu *ownerMenu, bool visible):_text(text),_ownerMenu(ownerMenu),_visible(visible){

}

QString MenuItem::getText(){
    return _text;
}

bool MenuItem::renderMenuItem(QRect renderRect, QPainter *qp, bool isCurrent)
{
    if(_visible){
        qp->drawText(renderRect,
                     Qt::AlignHCenter,
                    QString((isCurrent?"*":"") + _text));
    }
    return _visible;
}

Menu *MenuItem::getOwner()
{
    return _ownerMenu;
}

bool MenuItem::isVisible()
{
    return _visible;
}



Menu::Menu(QString title, Menu *ownerMenu, QString text, bool visible):_menuItems(8),_title(title), MenuItem((text != NULL)?text:title, ownerMenu, visible), _currentPos(-1){

}

QString Menu::getTitle()
{
    return _title;
}

void Menu::renderMenu(QRect renderRect, QPainter *qp)
{
    int headerFontSize = 50;
    int itemFontSize = 30;
    int itemOffset = 20;
    int yOffset = 120;


    //qp->fillRect(renderRect, QBrush(*Pixmap(WATER)));
    //Map bgMap(QCoreApplication::applicationDirPath() + QString(MAP_DIR) + "/menumap.fml");
    //bgMap.draw(qp,renderRect);



    qp->setFont(QFont(QString("sans serif"), headerFontSize, 10));
    qp->drawText(QRect(renderRect.topLeft(),QPoint(renderRect.right(), yOffset)),Qt::AlignHCenter,_title);
    qp->setFont(QFont(QString("sans serif"), itemFontSize, 10));
    for(int i = 0; i < _menuItems.size(); i++ ){

        assert(_menuItems.isValid(i));
        if((*_menuItems.getPointer(i))->renderMenuItem(QRect(renderRect.x(),
                                                  renderRect.y()+yOffset,
                                                  renderRect.width(),
                                                  itemFontSize+itemOffset),
                                                  qp,
                                                  (_currentPos==i))){
            yOffset += itemFontSize+itemOffset;
        }

    }
}

Menu *Menu::menuSelect()
{
    MenuItem *selectedItem = getSelected();
    return selectedItem->select();
}

void Menu::addMenuItem(MenuItem *item)
{
    _menuItems.add(item);
    if(_currentPos == -1){
        this->nextPos(true);
    }
}

MenuItem *Menu::getSelected()
{
    return *_menuItems.getPointer(_currentPos);
}

void Menu::nextPos(bool forward){

    if(_menuItems.size() == 0) return;
    int nextPos = _currentPos;
    int counter = 0;
    while(true){
            nextPos += (forward)? 1 : -1;

            if(nextPos == _currentPos || (nextPos == 0 && counter > 0 && _currentPos == -1)) break;

            if(nextPos < 0){
                nextPos = _menuItems.size();
            }else if(nextPos >= _menuItems.size()){
                nextPos = -1;
            }

            MenuItem *nextItem;
            if(_menuItems.get(nextPos, &nextItem) && nextItem->isVisible()){
                _currentPos = nextPos;
                break;
            }


            counter++;

     }
}


Menu *Menu::select()
{
    return this;
}



MenuAction::MenuAction(QString text, Menu *ownerMenu, void(*action)(), bool visible):MenuItem(text, ownerMenu, visible),_action(action){


}

Menu *MenuAction::select()
{
    if(_action) _action();
    return _ownerMenu;
}






LoadMapMenuAction::LoadMapMenuAction(QString text, Menu *ownerMenu, Game *game, QString mapFilename, bool visible): MenuAction(text, ownerMenu, NULL, visible), _game(game), _mapFilename(mapFilename)
{

}

Menu *LoadMapMenuAction::select()
{
    _game->loadMap(_mapFilename);
    return NULL;
}
