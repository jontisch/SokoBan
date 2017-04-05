#ifndef EDITORWIDGET_H
#define EDITORWIDGET_H
#include "../collection.h"
#include "../map/tile.h"
#include <QPainter>
#include <QPixmap>
#include <QDebug>

class EditorWidget
{
public:
    EditorWidget();
};


class ListEditorWidget: public EditorWidget
{
public:
    ListEditorWidget(QString title);
    QRect *getArea();
    void addItem(QString *item, QPixmap *pixmap);
    void renderWidget(QPainter *painter, QRect renderRect);
    void select(int height);
    int getSelected();

private:

    struct ListRenderingMeasurements
    {
        int padding;
        int itemOffset;
        int itemFontSize;
        int itemHeight;
        int headerFontSize;
        int headerHeight;
        int iconWidth;
    };

    QRect calculateItemRenderRect(int index, ListRenderingMeasurements *m);
    ListEditorWidget::ListRenderingMeasurements calculateRenderingMeasurements(QRect renderRect);


    QString _title;
    Collection<QString*> _listNames;
    Collection<QPixmap*> _listThumbs;

    QRect _area;
    int _selected;

    int heightToIndex(int height);
};



#endif // EDITORWIDGET_H
