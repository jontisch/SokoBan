#include "listeditorwidget.h"

ListEditorWidget::ListEditorWidget(QString title, QSize gridUnits, QPoint gridPos):EditorWidget(title, gridUnits, gridPos)
{

}

void ListEditorWidget::addItem(QString *item, QPixmap *pixmap)
{
    _listNames.add(&item);
    _listThumbs.add(&pixmap);
}

void ListEditorWidget::renderWidget(QPainter *painter, QRect renderRect, QLinearGradient *gradient)
{

    ListRenderingMeasurements m = calculateRenderingMeasurements(renderRect);

    _area = renderRect;
    painter->setBrush(Qt::black);
    painter->setPen(Qt::white);
    painter->drawRect(renderRect);
    painter->setFont(QFont(QString("sans serif"), m.headerFontSize, 75));
    //HMM
    QRect headerRect = QRect(_area.left()+m.padding,
                     _area.top()+m.padding,
                     _area.width()-2*m.padding,
                     m.headerHeight);

    gradient->setStart(headerRect.topLeft());
    gradient->setFinalStop(headerRect.bottomLeft());
    painter->setPen(QColor(6,28,43));
    painter->setBrush(*gradient);


    painter->drawRoundedRect(headerRect, 10,10);
    //HMM
    painter->drawText(headerRect, Qt::AlignCenter,_title);
    painter->setPen(Qt::white);
    painter->setBrush(Qt::black);
    for(int i = 0; i < _listNames.N; i++){

        painter->setFont(QFont(QString("sans serif"), m.itemFontSize, (i == _selected)?75:50));
        painter->drawPixmap(_area.left()+m.padding,
                            _area.top()+m.padding + m.headerHeight + i * (m.itemHeight+m.itemOffset),
                            m.iconWidth,
                            m.itemHeight,
                            **_listThumbs.getPointer(i));
        painter->drawText(QRect(_area.left()+m.padding+m.iconWidth+m.padding,
                                _area.top()+m.padding + m.headerHeight + i * (m.itemHeight+m.itemOffset),
                                _area.width()-3*m.padding - m.iconWidth,
                                m.itemHeight),Qt::AlignVCenter, **_listNames.getPointer(i));
    }
}

void ListEditorWidget::select(int height){
    int index = heightToIndex(height);
    if(index < _listNames.N && index >= 0){
        _selected = index;
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
    result.padding = 4;
    result.itemOffset = 3;
    result.itemFontSize = 10;
    result.itemHeight = 24;
    result.headerFontSize = 12;
    result.headerHeight = 30;
    result.iconWidth = 24;

    return result;
}

int ListEditorWidget::heightToIndex(int height){
    ListRenderingMeasurements m = calculateRenderingMeasurements(_area);

    return (height - (_area.top() + m.padding + m.headerHeight)) / (m.itemHeight+m.itemOffset);
}
