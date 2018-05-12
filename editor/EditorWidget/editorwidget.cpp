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

QPoint *EditorWidget::getGridPos()
{
    return &_gridPos;
}

void EditorWidget::setHover(bool hover)
{
    _hover = hover;
}













