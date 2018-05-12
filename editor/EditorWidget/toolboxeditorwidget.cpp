#include "toolboxeditorwidget.h"

ToolboxEditorWidget::ToolboxEditorWidget(QString title, QSize gridSize, QSize gridUnits, QPoint gridPos):EditorWidget(title, gridUnits, gridPos), _gridSize(gridSize)
{

}

void ToolboxEditorWidget::addWidget(EditorWidget *widget)
{
    _widgets.add(&widget);
}

void ToolboxEditorWidget::renderWidget(QPainter *painter, QRect renderRect, QLinearGradient *gradient)
{
    _area = renderRect;
    QRect widgetRect;
    painter->fillRect(_area, Qt::black);
    for(int i = 0; i < _widgets.N; i++){
        EditorWidget *w = _widgets.E[i];

        widgetRect = QRect(renderRect.left() + (renderRect.width()/_gridSize.width()) * w->getGridPos()->x(),
                           renderRect.top() + (renderRect.height()/_gridSize.height()) * w->getGridPos()->y(),
                           (renderRect.width()/ _gridSize.width()) * w->getGridUnits()->width(),
                           (renderRect.height()/ _gridSize.height()) * w->getGridUnits()->height()
                           );
        w->renderWidget(painter, widgetRect, gradient);
    }

}
