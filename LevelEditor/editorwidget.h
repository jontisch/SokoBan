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
    void setHover(bool hover);
protected:
    QString _title;
    QRect _area;
    bool _hover;
};

class RadioEditorWidget: public EditorWidget
{
public:
    RadioEditorWidget(QString title, bool state);
    void renderWidget(QPainter *painter, QRect renderRect);
    bool getState();
    void setState(bool state);
private:
    QString _text;
    bool _state;
};

class RadioClusterEditorWidget: public EditorWidget
{
public:
    RadioClusterEditorWidget(QString title, int index);
    void addRadio(RadioEditorWidget *radio);
    RadioEditorWidget* getPointer(int index);
    void renderWidget(QPainter *painter, QRect renderRect);
    int select(int x, int y, int index = -1);
private:
    int _index;
    Collection<RadioEditorWidget*> _radios;
};

class ButtonEditorWidget: public EditorWidget{
public:
    ButtonEditorWidget(QString title, bool active = true);
    bool active();
    void renderWidget(QPainter *painter, QRect renderRect);
private:
    bool _active;
};

class LabelEditorWidget: public EditorWidget{
public:
    LabelEditorWidget(QString title, QString text = NULL);
    void setText(QString newText);
    void renderWidget(QPainter *painter, QRect renderRect);
private:
    QString _text;
};

class TextfieldEditorWidget: public EditorWidget{
public:
    TextfieldEditorWidget(QString title, QString text);
    void addChar(QChar c);
    void backSpace();
    void setText(QString text);
    void renderWidget(QPainter *painter, QRect renderRect);
    void setEdit(bool edit);
    QString getText();
private:
    bool _editActive;
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
