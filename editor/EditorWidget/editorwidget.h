#ifndef EDITORWIDGET_H
#define EDITORWIDGET_H
#include "../../collections.h"
#include "../../map/tile.h"
#include "../../global.h"
#include <QPainter>
#include <QPixmap>
#include <QDebug>


class EditorWidget
{
public:
    EditorWidget(QString title, QSize gridUnits = QSize(1,1), QPoint gridPos = QPoint(0,0));
    virtual void renderWidget(QPainter *painter, QRect renderRect, QLinearGradient *gradient) = 0;
    QRect *getArea();
    QSize *getGridUnits();
    QPoint *getGridPos();
    void setHover(bool hover);
protected:
    QString _title;
    QRect _area;
    QSize _gridUnits;
    QPoint _gridPos;
    bool _hover;
};


#endif // EDITORWIDGET_H
