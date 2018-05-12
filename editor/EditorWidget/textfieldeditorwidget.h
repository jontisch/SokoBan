#ifndef TEXTFIELDEDITORWIDGET_H
#define TEXTFIELDEDITORWIDGET_H

#include "editorwidget.h"


class TextfieldEditorWidget: public EditorWidget{
public:
    TextfieldEditorWidget(QString title, QString text, QSize gridUnits = QSize(1,1), QPoint gridPos = QPoint(0,0), bool numeric = false);
    void addChar(QChar c);
    void backSpace();
    void setText(QString text);
    void renderWidget(QPainter *painter, QRect renderRect, QLinearGradient *gradient);
    void setEdit(bool edit);
    QString getText();
private:
    bool _editActive;
    bool _numeric;
    QString _text;
};

#endif // TEXTFIELDEDITORWIDGET_H
