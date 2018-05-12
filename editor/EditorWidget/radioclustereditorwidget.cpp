#include "radioclustereditorwidget.h"


RadioClusterEditorWidget::RadioClusterEditorWidget(QString title, int index, QSize gridUnits, QPoint gridPos):EditorWidget(title, gridUnits, gridPos), _index(index)
{

}

void RadioClusterEditorWidget::addRadio(RadioEditorWidget *radio)
{
    _radios.add(&radio);
}

RadioEditorWidget *RadioClusterEditorWidget::getPointer(int index)
{
    if(index > _radios.N) return NULL;
    return _radios.E[index];
}



void RadioClusterEditorWidget::renderWidget(QPainter *painter, QRect renderRect, QLinearGradient *gradient)
{
    _area = renderRect;
    int count = _radios.N;
    painter->setBrush(Qt::black);
    painter->setPen(Qt::white);
    painter->drawRect(renderRect);
    for(int i = 0; i < count; i++){
        RadioEditorWidget *radio = _radios.E[i];
        radio->renderWidget(painter, QRect(renderRect.left()+i*renderRect.width()/count, renderRect.top(), renderRect.width()/count, renderRect.height()), gradient);
    }
}

int RadioClusterEditorWidget::select(int x, int y, int index)
{
    if(index > -1){
        _index = index;

    }else {
        _index = x/(_area.width()/_radios.N);
    }
    for(int i = 0; i < _radios.N; i++ ){
        RadioEditorWidget *radio = _radios.E[i];
        radio->setState(_index == i);
    }
    return _index;
}

