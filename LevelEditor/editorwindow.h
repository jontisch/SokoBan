#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include "../map.h"
#include <QMainWindow>
#include <QPaintEvent>
#include <QMouseEvent>

namespace Ui {
class EditorWindow;
}

class EditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditorWindow(QWidget *parent = 0);
    ~EditorWindow();

    void mousePressEvent(QMouseEvent *Event);
    void mouseMoveEvent(QMouseEvent *Event);
    void paintEvent(QPaintEvent *Event);

private slots:
    void on_list_tile_types_itemSelectionChanged();

    void on_save_button_clicked(bool checked);

private:
    Ui::EditorWindow *ui;
    Map *map;

    QPoint lastClickedTile;
    Tiletype selectedTileType;

    QRect mapArea();
};

#endif // EDITORWINDOW_H
