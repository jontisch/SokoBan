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
        qp->drawText(renderRect, Qt::AlignCenter, _text);
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

void MenuItem::setVisible(bool visible)
{
    _visible = visible;
}



Menu::Menu(QString title, Menu *ownerMenu, QString text, bool visible):_menuItems(8),_title(title), MenuItem((text != NULL)?text:title, ownerMenu, visible), _currentPos(-1){
    QString appPath = QCoreApplication::applicationDirPath() + MAP_DIR;
    _bgMap = new Map(MapFilename("menumap.fml"));
    _bgMap->setPlayerVisible(false);

}

QString Menu::getTitle()
{
    return _title;
}

void Menu::renderMenu(QRect renderRect, QPainter *qp)
{
    MenuRenderingMeasurements m = calculateRenderingMeasurements(renderRect);


    _bgMap->draw(qp, renderRect);



    qp->setFont(QFont(QString("sans serif"), m.headerFontSize, 10));
    qp->drawText(QRect(m.itemX,m.menuOffset,m.itemWidth,m.itemHeight),Qt::AlignCenter,_title);
    qp->setFont(QFont(QString("sans serif"), m.itemFontSize, 10));
    int c = 0;
    for(int i = 0; i < _menuItems.size(); i++ ){

        assert(_menuItems.isValid(i));

        if((*_menuItems.getPointer(i))->renderMenuItem(calculateItemRenderRect(c, &m), qp, (_currentPos==i))){
            c++;
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

void Menu::setPos(int pos)
{
    _currentPos = pos;
    MenuItem *nextItem;
    if(!_menuItems.get(pos, &nextItem) || !nextItem->isVisible()) nextPos(true);
    else updateMap();
}

Map *Menu::getMap()
{
    return _bgMap;
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

    updateMap();
}


Menu *Menu::select()
{
    return this;
}

Menu::MenuRenderingMeasurements Menu::calculateRenderingMeasurements(QRect renderRect)
{
    MenuRenderingMeasurements result;
    result.tileSize = _bgMap->calculateTileSize(renderRect);
    result.topPadding = _bgMap->calculatePixelOffset(result.tileSize, renderRect).y();
    result.menuOffset = 2.8*result.tileSize + result.topPadding;
    result.itemHeight = 3*result.tileSize;
    result.itemWidth = 10*result.tileSize;
    result.spacing = result.tileSize;
    result.itemX = renderRect.x()+renderRect.width()/2-result.itemWidth/2;
    result.headerFontSize = 1.25*result.tileSize;
    result.itemFontSize = result.tileSize;

    return result;
}

QRect Menu::calculateItemRenderRect(int index, MenuRenderingMeasurements *m)
{
    return QRect(m->itemX,
                 m->menuOffset+(index+1)*(m->itemHeight+m->spacing),
                 m->itemWidth,
                 m->itemHeight);
}

void Menu::updateMap(){

    MenuItem *item;
    int c = 0;
    int i = 0;
    while(_menuItems.get(i, &item))
    {
        if(item->isVisible()){
            QPoint tile(_bgMap->width()/2 - 10/2, 3 + 4*(c+1) + 3/2);
            if(i == _currentPos){
                getMap()->addTileFlag(tile.x(), tile.y(), HAS_BOX);
                getMap()->addTileFlag(tile.x()+9, tile.y(), HAS_BOX);
            } else {
                getMap()->removeTileFlag(tile.x(), tile.y(), HAS_BOX);
                getMap()->removeTileFlag(tile.x()+9, tile.y(), HAS_BOX);
            }
            c++;

        }
        i++;
    }

}

MenuAction::MenuAction(QString text, Menu *ownerMenu, int action, MenuActionDelegate *delegate, bool visible): MenuItem(text, ownerMenu, visible),_action(action),_delegate(delegate){


}

Menu *MenuAction::select()
{
    _delegate->executeMenuAction(_action);
    return _ownerMenu;
}

