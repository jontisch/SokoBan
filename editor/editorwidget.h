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
    EditorWidget(QString title, QSize gridUnits = QSize(1,1), QPoint gridPos = QPoint(0,0));
    virtual void renderWidget(QPainter *painter, QRect renderRect, QLinearGradient *gradient) = 0;
    QRect *getArea();
    QSize *getGridUnits();
    QPoint *getgridPos();
    void setHover(bool hover);
protected:
    QString _title;
    QRect _area;
    QSize _gridUnits;
    QPoint _gridPos;
    bool _hover;
};







class ToolboxEditorWidget: public EditorWidget
{
public:
    ToolboxEditorWidget(QString title, QSize gridSize, QSize gridUnits = QSize(1,1), QPoint gridPos = QPoint(0,0));
    void addWidget(EditorWidget *widget);
    void renderWidget(QPainter *painter, QRect renderRect, QLinearGradient *gradient);
private:
    QSize _gridSize;
    Collection<EditorWidget*> _widgets;
};









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
    Collection<RadioEditorWidget*> _radios;
};








class ButtonEditorWidget: public EditorWidget{
public:
    ButtonEditorWidget(QString title, bool active = true, QSize gridUnits = QSize(1,1), QPoint gridPos = QPoint(0,0));
    bool active();
    void renderWidget(QPainter *painter, QRect renderRect, QLinearGradient *gradient);
private:
    bool _active;
};








class LabelEditorWidget: public EditorWidget{
public:
    LabelEditorWidget(QString title, QString text = NULL, QSize gridUnits = QSize(1,1), QPoint gridPos = QPoint(0,0));
    void setText(QString newText);
    void renderWidget(QPainter *painter, QRect renderRect, QLinearGradient *gradient);
private:
    QString _text;
};








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








class ListEditorWidget: public EditorWidget
{
public:
    ListEditorWidget(QString title, QSize gridUnits = QSize(1,1), QPoint gridPos = QPoint(0,0));
    void addItem(QString *item, QPixmap *pixmap);
    void renderWidget(QPainter *painter, QRect renderRect, QLinearGradient *gradient);
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
