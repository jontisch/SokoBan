#ifndef LISTEDITORWIDGET_H
#define LISTEDITORWIDGET_H
#include "editorwidget.h"

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
    List<QString*> _listNames = {0};
    List<QPixmap*> _listThumbs = {0};
    int _selected;
    int heightToIndex(int height);
};

#endif // LISTEDITORWIDGET_H
