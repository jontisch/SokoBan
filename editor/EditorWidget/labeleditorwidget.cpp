#include "labeleditorwidget.h"

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
