#ifndef BUTTONEDITORWIDGET_H
#define BUTTONEDITORWIDGET_H
#include "editorwidget.h"

class ButtonEditorWidget: public EditorWidget{
public:
    ButtonEditorWidget(QString title, bool active = true, QSize gridUnits = QSize(1,1), QPoint gridPos = QPoint(0,0));
    bool active();
    void renderWidget(QPainter *painter, QRect renderRect, QLinearGradient *gradient);
private:
    bool _active;
};

#endif // BUTTONEDITORWIDGET_H
