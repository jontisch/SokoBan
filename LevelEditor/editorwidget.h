#ifndef EDITORWIDGET_H
#define EDITORWIDGET_H
#include "../collection.h"
#include "../map/tile.h"
#include "../global.h"
#include <QPainter>
#include <QPixmap>
#include <QDebug>

class EditorWidget
{
public:
    EditorWidget(QString title);
    QRect *getArea();

protected:
    QString _title;
    QRect _area;

};

class RadioEditorWidget: public EditorWidget
{
public:
    RadioEditorWidget(QString title, bool state);
    void renderWidget(QPainter *painter, QRect renderRect);
    bool getState();
    void toggleState();

private:
    QString _text;
    bool _state;

};

class LabelEditorWidget: public EditorWidget{
public:
    LabelEditorWidget(QString title, QString text);
    void setText(QString newText);
    void renderWidget(QPainter *painter, QRect renderRect);
private:
    QString _text;
};

class ListEditorWidget: public EditorWidget
{
public:
    ListEditorWidget(QString title);
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


    Collection<QString*> _listNames;
    Collection<QPixmap*> _listThumbs;

    int _selected;

    int heightToIndex(int height);
};



#endif // EDITORWIDGET_H
