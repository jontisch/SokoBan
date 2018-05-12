#ifndef RADIOCLUSTEREDITORWIDGET_H
#define RADIOCLUSTEREDITORWIDGET_H

#include "editorwidget.h"
#include "radioeditorwidget.h"

class RadioClusterEditorWidget: public EditorWidget
{
public:
    RadioClusterEditorWidget(QString title, int index, QSize gridUnits = QSize(1,1), QPoint gridPos = QPoint(0,0));
    void addRadio(RadioEditorWidget *radio);
    RadioEditorWidget* getPointer(int index);
    void renderWidget(QPainter *painter, QRect renderRect, QLinearGradient *gradient);
    int select(int x, int y, int index = -1);
private:
    int _index;
    List<RadioEditorWidget*> _radios = {0};
};


#endif // RADIOCLUSTEREDITORWIDGET_H
