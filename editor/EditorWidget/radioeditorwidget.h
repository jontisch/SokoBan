#ifndef RADIOEDITORWIDGET_H
#define RADIOEDITORWIDGET_H

#include "editorwidget.h"

class RadioEditorWidget: public EditorWidget
{
public:
    RadioEditorWidget(QString title, bool state, QSize gridUnits = QSize(1,1), QPoint gridPos = QPoint(0,0));
    void renderWidget(QPainter *painter, QRect renderRect, QLinearGradient *gradient);
    bool getState();
    void setState(bool state);
private:
    QString _text;
    bool _state;
};

#endif // RADIOEDITORWIDGET_H
