#include "buttoneditorwidget.h"

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
