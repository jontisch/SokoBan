#include "editorwidget.h"

EditorWidget::EditorWidget(QString title):_title(title), _area()
{

}


QRect *EditorWidget::getArea()
{
    return &_area;
}


ListEditorWidget::ListEditorWidget(QString title):EditorWidget(title), _listNames(32), _listThumbs(32)
{

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
    int index = heightToIndex(height);
    if(index < _listNames.count() && index >= 0){
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
    ListRenderingMeasurements m = calculateRenderingMeasurements(_area);

    return (height - (_area.top() + m.padding + m.headerHeight)) / (m.itemHeight+m.itemOffset);
}

LabelEditorWidget::LabelEditorWidget(QString title, QString text):EditorWidget(title), _text(text)
{

}

void LabelEditorWidget::setText(QString newText)
{
    _text = newText;
}

void LabelEditorWidget::renderWidget(QPainter *painter, QRect renderRect)
{
    _area = renderRect;
    painter->fillRect(renderRect, Qt::black);
    painter->setPen(Qt::white);
    painter->setFont(QFont(QString("sans serif"), 10, 10));
    painter->drawText(renderRect, _text);

}

RadioEditorWidget::RadioEditorWidget(QString title, bool state):EditorWidget(title), _state(state)
{

}

void RadioEditorWidget::renderWidget(QPainter *painter, QRect renderRect)
{
    _area = renderRect;
    painter->fillRect(renderRect, Qt::black);
    painter->setPen(Qt::white);
    painter->setFont(QFont(QString("sans serif"), 10, 10));
    painter->drawText(renderRect, _title);
    painter->drawPixmap(renderRect.left()+10,renderRect.bottom()-34,24,24,(_state)?*Pixmap(PIXMAP_BOX):*Pixmap(PIXMAP_FLOOR));
}

bool RadioEditorWidget::getState()
{
    return _state;
}

void RadioEditorWidget::setState(bool state)
{
    _state = state;
}

RadioClusterEditorWidget::RadioClusterEditorWidget(QString title, int index):EditorWidget(title), _index(index),_radios(4)
{

}

void RadioClusterEditorWidget::addRadio(RadioEditorWidget *radio)
{
    _radios.add(radio);
}

RadioEditorWidget *RadioClusterEditorWidget::getPointer(int index)
{
    RadioEditorWidget *result = NULL;
    return (_radios.get(index, &result)?result:NULL);
}



void RadioClusterEditorWidget::renderWidget(QPainter *painter, QRect renderRect)
{
    _area = renderRect;
    int count = _radios.count();
    painter->fillRect(renderRect, Qt::black);
    for(int i = 0; i < count; i++){
        RadioEditorWidget *radio;
        _radios.get(i, &radio);
        radio->renderWidget(painter, QRect(renderRect.left()+i*renderRect.width()/count, renderRect.top(), renderRect.width()/count, renderRect.height()));
    }
}

int RadioClusterEditorWidget::select(int x, int y, int index)
{
    if(index > -1){
        _index = index;

    }else {
        _index = x/(_area.width()/_radios.count());
    }
    for(int i = 0; i < _radios.count(); i++ ){
        RadioEditorWidget *radio;
        _radios.get(i, &radio);
        radio->setState(_index == i);
    }
    return _index;
}
