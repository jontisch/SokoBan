#include "radioeditorwidget.h"

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
