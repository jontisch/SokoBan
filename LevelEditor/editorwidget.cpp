#include "editorwidget.h"

EditorWidget::EditorWidget()
{

}

ListEditorWidget::ListEditorWidget(QString title):_title(title), _area(), _listNames(32), _listThumbs(32)
{

}

QRect *ListEditorWidget::getArea()
{
    return &_area;
}

void ListEditorWidget::addItem(QString *item, QPixmap *pixmap)
{
    _listNames.add(item);
    _listThumbs.add(pixmap);

}

void ListEditorWidget::renderWidget(QPainter *painter, QRect renderRect)
{

    ListRenderingMeasurements m = calculateRenderingMeasurements(renderRect);

    _area = renderRect;

    painter->fillRect(_area, Qt::black);
    painter->setPen(Qt::white);
    painter->setFont(QFont(QString("sans serif"), m.headerFontSize, 10));
    painter->drawText(QRect(_area.left()+m.padding,
                            _area.top()+m.padding,
                            _area.width()-2*m.padding,
                            m.headerHeight), Qt::AlignCenter,_title);

    painter->setFont(QFont(QString("sans serif"), m.itemFontSize, 10));
    for(int i = 0; i < _listNames.count(); i++){
        QRect rect = calculateItemRenderRect(i, &m);
        painter->fillRect(rect, Qt::gray);
        painter->drawPixmap(_area.left()+m.padding,
                            _area.top()+m.padding + m.headerHeight + i * (m.itemHeight+m.itemOffset),
                            m.iconWidth,
                            m.itemHeight,
                            **_listThumbs.getPointer(i));
        painter->drawText(QRect(_area.left()+m.padding+m.iconWidth+m.padding,
                                _area.top()+m.padding + m.headerHeight + i * (m.itemHeight+m.itemOffset),
                                _area.width()-3*m.padding - m.iconWidth,
                                m.itemHeight),((_selected == i)?Qt::AlignRight:Qt::AlignVCenter), **_listNames.getPointer(i));
    }
}

void ListEditorWidget::select(int height){
    if(heightToIndex(height) < _listNames.count()){
        _selected = heightToIndex(height);
    }

}

int ListEditorWidget::getSelected(){
    return _selected;
}

QRect ListEditorWidget::calculateItemRenderRect(int index, ListEditorWidget::ListRenderingMeasurements *m)
{
    return QRect(_area.left()+m->padding,
                 _area.top()+m->padding + m->headerHeight + index * (m->itemHeight+m->itemOffset),
                 _area.width()-2*m->padding,
                 m->itemHeight);
}

ListEditorWidget::ListRenderingMeasurements ListEditorWidget::calculateRenderingMeasurements(QRect renderRect)
{
    ListRenderingMeasurements result;
    result.padding = 10;
    result.itemOffset = 3;
    result.itemFontSize = 10;
    result.itemHeight = 24;
    result.headerFontSize = 22;
    result.headerHeight = 30;
    result.iconWidth = 24;

    return result;
}

int ListEditorWidget::heightToIndex(int height){
    return height / 30;
}
