#include "editorwidget.h"

EditorWidget::EditorWidget()
{

}

ListEditorWidget::ListEditorWidget(QString title):_title(title), _listNames(32), _listThumbs(32)
{

}

void ListEditorWidget::addItem(QString *item, QPixmap *pixmap)
{
    _listNames.add(item);
    _listThumbs.add(pixmap);
}

void ListEditorWidget::renderWidget(QRect renderRect, QPainter *painter)
{
    painter->fillRect(renderRect, Qt::green);
    for(int i = 0; i < _listNames.count(); i++){
        painter->drawText(renderRect.left()+45, renderRect.top()+ i*40, renderRect.width()-45, 40,Qt::AlignVCenter, **_listNames.getPointer(i));
    }
    for(int i = 0; i < _listThumbs.count(); i++){
        painter->drawPixmap(renderRect.left(), renderRect.top()+ i*40, 40, 40, **_listThumbs.getPointer(i));
    }
}
