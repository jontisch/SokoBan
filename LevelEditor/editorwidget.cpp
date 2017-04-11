#include "editorwidget.h"

EditorWidget::EditorWidget(QString title, QSize gridUnits, QPoint gridPos):_title(title), _gridUnits(gridUnits), _gridPos(gridPos), _area(), _hover(false)
{

}


QRect *EditorWidget::getArea()
{
    return &_area;
}

QSize *EditorWidget::getGridUnits()
{
    return &_gridUnits;
}

QPoint *EditorWidget::getgridPos()
{
    return &_gridPos;
}

void EditorWidget::setHover(bool hover)
{
    _hover = hover;
}









ListEditorWidget::ListEditorWidget(QString title, QSize gridUnits, QPoint gridPos):EditorWidget(title, gridUnits, gridPos), _listNames(32), _listThumbs(32)
{

}

void ListEditorWidget::addItem(QString *item, QPixmap *pixmap)
{
    _listNames.add(item);
    _listThumbs.add(pixmap);
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
    for(int i = 0; i < _listNames.count(); i++){

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














LabelEditorWidget::LabelEditorWidget(QString title, QString text, QSize gridUnits, QPoint gridPos):EditorWidget(title, gridUnits, gridPos), _text(text)
{

}

void LabelEditorWidget::setText(QString newText)
{
    _text = newText;
}

void LabelEditorWidget::renderWidget(QPainter *painter, QRect renderRect, QLinearGradient *gradient)
{
    _area = renderRect;
    //painter->setPen(Qt::white);
    //painter->setBrush(Qt::black);
    //painter->drawRoundedRect(renderRect, 10,10);
    gradient->setStart(_area.topLeft());
    gradient->setFinalStop(_area.bottomLeft());
    painter->setPen(QColor(6,28,43));
    painter->setBrush(*gradient);
    painter->drawRoundedRect(renderRect, 10,10);
    painter->setFont(QFont(QString("sans serif"), 11, 75));
    painter->drawText(renderRect, (_text == NULL)?Qt::AlignCenter:Qt::AlignHCenter, _title);
    if(_text != NULL){
        painter->setFont(QFont(QString("sans serif"), 10, 50));
        painter->drawText(renderRect, Qt::AlignHCenter | Qt::AlignBottom, _text);
    }
}














RadioEditorWidget::RadioEditorWidget(QString title, bool state, QSize gridUnits, QPoint gridPos):EditorWidget(title, gridUnits, gridPos), _state(state)
{

}

void RadioEditorWidget::renderWidget(QPainter *painter, QRect renderRect, QLinearGradient *gradient)
{
    _area = renderRect;
    painter->fillRect(renderRect, Qt::black);
    painter->setPen(Qt::white);
    painter->drawRect(renderRect);
    painter->setFont(QFont(QString("sans serif"), 10, 10));
    painter->drawText(renderRect, Qt::AlignHCenter, _title);
    painter->drawPixmap(renderRect.left()+(renderRect.width()-24)/2,renderRect.bottom()-30,24,24,(_state)?*Pixmap(PIXMAP_BOX):*Pixmap(PIXMAP_FLOOR));
}

bool RadioEditorWidget::getState()
{
    return _state;
}

void RadioEditorWidget::setState(bool state)
{
    _state = state;
}











RadioClusterEditorWidget::RadioClusterEditorWidget(QString title, int index, QSize gridUnits, QPoint gridPos):EditorWidget(title, gridUnits, gridPos), _index(index),_radios(4)
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



void RadioClusterEditorWidget::renderWidget(QPainter *painter, QRect renderRect, QLinearGradient *gradient)
{
    _area = renderRect;
    int count = _radios.count();
    painter->setBrush(Qt::black);
    painter->setPen(Qt::white);
    painter->drawRect(renderRect);
    for(int i = 0; i < count; i++){
        RadioEditorWidget *radio;
        _radios.get(i, &radio);
        radio->renderWidget(painter, QRect(renderRect.left()+i*renderRect.width()/count, renderRect.top(), renderRect.width()/count, renderRect.height()), gradient);
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










ButtonEditorWidget::ButtonEditorWidget(QString title, bool active, QSize gridUnits, QPoint gridPos):EditorWidget(title, gridUnits, gridPos), _active(active)
{

}

bool ButtonEditorWidget::active()
{
    return _active;
}

void ButtonEditorWidget::renderWidget(QPainter *painter, QRect renderRect, QLinearGradient *gradient)
{
    _area = renderRect;


    //painter->setPen((_hover)?((_active)?Qt::black:Qt::gray):((_active)?Qt::white:Qt::gray));
    //painter->setBrush((_hover)?((_active)?Qt::white:Qt::black):Qt::black);
    gradient->setStart(_area.topLeft());
    gradient->setFinalStop(_area.bottomLeft());
    painter->setPen((_hover)?QColor(93,121,138):QColor(6,28,43));
    painter->setBrush(*gradient);
    painter->drawRoundedRect(renderRect, 10,10);
    painter->setFont(QFont(QString("sans serif"), 10, 10));
    painter->drawText(renderRect, Qt::AlignCenter, _title);
}









TextfieldEditorWidget::TextfieldEditorWidget(QString title, QString text, QSize gridUnits, QPoint gridPos):EditorWidget(title, gridUnits, gridPos), _text(text), _editActive(false)
{

}

void TextfieldEditorWidget::addChar(QChar c)
{
    _text.append(c);
    if(_text.toInt()>128){
        _text == "128";
    }
    else if(_text.toInt()<0){
        _text == "0";
    }
}

void TextfieldEditorWidget::backSpace()
{
    _text.truncate(_text.length()-1);
    if(_text.length() == 0){
        _text = "0";
    }
}

void TextfieldEditorWidget::setText(QString text)
{
    _text = text;
}



void TextfieldEditorWidget::renderWidget(QPainter *painter, QRect renderRect, QLinearGradient *gradient)
{
    _area = renderRect;
    //painter->setPen((_editActive||_hover)?Qt::black:Qt::white);
    //painter->setBrush((_editActive||_hover)?Qt::white:Qt::black);
    gradient->setStart(_area.topLeft());
    gradient->setFinalStop(_area.bottomLeft());
    painter->setPen(QColor(6,28,43));
    painter->setBrush(*gradient);
    painter->drawRoundedRect(renderRect, 10,10);
    painter->setFont(QFont(QString("sans serif"), 10, 10));
    painter->drawText(renderRect, Qt::AlignHCenter, _title);
    painter->drawText(renderRect, Qt::AlignBottom | Qt::AlignHCenter, _text);
}

void TextfieldEditorWidget::setEdit(bool edit)
{
    _editActive = edit;
}

QString TextfieldEditorWidget::getText()
{
    return _text;
}










ToolboxEditorWidget::ToolboxEditorWidget(QString title, QSize gridSize, QSize gridUnits, QPoint gridPos):EditorWidget(title, gridUnits, gridPos), _gridSize(gridSize), _widgets(16)
{

}

void ToolboxEditorWidget::addWidget(EditorWidget *widget)
{
    _widgets.add(widget);
}

void ToolboxEditorWidget::renderWidget(QPainter *painter, QRect renderRect, QLinearGradient *gradient)
{
    _area = renderRect;
    QRect widgetRect;
    painter->fillRect(_area, Qt::black);
    for(int i = 0; i < _widgets.count(); i++){
        EditorWidget *w;
        _widgets.get(i, &w);

        widgetRect = QRect(renderRect.left() + (renderRect.width()/_gridSize.width()) * w->getgridPos()->x(),
                           renderRect.top() + (renderRect.height()/_gridSize.height()) * w->getgridPos()->y(),
                           (renderRect.width()/ _gridSize.width()) * w->getGridUnits()->width(),
                           (renderRect.height()/ _gridSize.height()) * w->getGridUnits()->height()
                           );
        w->renderWidget(painter, widgetRect, gradient);
    }

}
