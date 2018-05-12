#ifndef TOOLBOXEDITORWIDGET_H
#define TOOLBOXEDITORWIDGET_H
#include "editorwidget.h"

class ToolboxEditorWidget: public EditorWidget
{
public:
    ToolboxEditorWidget(QString title, QSize gridSize, QSize gridUnits = QSize(1,1), QPoint gridPos = QPoint(0,0));
    void addWidget(EditorWidget *widget);
    void renderWidget(QPainter *painter, QRect renderRect, QLinearGradient *gradient);
private:
    QSize _gridSize;
    List<EditorWidget*> _widgets = {0};
};

#endif // TOOLBOXEDITORWIDGET_H
