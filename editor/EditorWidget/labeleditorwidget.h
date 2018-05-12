#ifndef LABELEDITORWIDGET_H
#define LABELEDITORWIDGET_H

#include "editorwidget.h"

class LabelEditorWidget: public EditorWidget{
public:
    LabelEditorWidget(QString title, QString text = NULL, QSize gridUnits = QSize(1,1), QPoint gridPos = QPoint(0,0));
    void setText(QString newText);
    void renderWidget(QPainter *painter, QRect renderRect, QLinearGradient *gradient);
private:
    QString _text;
};

#endif // LABELEDITORWIDGET_H
