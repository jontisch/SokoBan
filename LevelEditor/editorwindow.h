#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include "../map.h"
#include <QMainWindow>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QListWidgetItem>

namespace Ui {
class EditorWindow;
}

enum EditingMode
{
    SET_TYPE,
    ADD_FLAG,
    REMOVE_FLAG
};


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

    void on_radio_set_type_clicked();

    void on_radio_add_flag_clicked();

    void on_radio_remove_flag_clicked();

    void on_list_flags_currentRowChanged(int currentRow);

    void on_list_flags_itemClicked(QListWidgetItem *item);

    void on_list_tile_types_itemClicked(QListWidgetItem *item);

private:
    Ui::EditorWindow *ui;
    Map *map;

    QPoint lastClickedTile;
    TileFlag selectedFlag;
    TileType selectedTileType;

    void operateOnTile(QPoint tile);

    TileType tileTypeForListRow[N_TILETYPES];

    QRect mapArea();

    EditingMode mode;
};

#endif // EDITORWINDOW_H
