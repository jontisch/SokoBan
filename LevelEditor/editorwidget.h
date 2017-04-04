#ifndef EDITORWIDGET_H
#define EDITORWIDGET_H
#include "../collection.h"
#include "../map/tile.h"
#include <QPainter>
#include <QPixmap>

class EditorWidget
{
public:
    EditorWidget();
};


class ListEditorWidget: public EditorWidget
{
public:
    ListEditorWidget(QString title);
    void addItem(QString *item, QPixmap *pixmap);
    void renderWidget(QRect renderRect, QPainter *painter);
private:
    QString _title;

    Collection<QString*> _listNames;
    Collection<QPixmap*> _listThumbs;

    int _selected;

};



#endif // EDITORWIDGET_H
