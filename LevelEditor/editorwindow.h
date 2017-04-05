#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include "../map/map.h"
#include "../map/tile.h"
#include <QMainWindow>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QListWidgetItem>
#include "editorwidget.h"

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

    void on_button_update_size_clicked();

    void on_slider_height_valueChanged(int value);

    void on_slider_width_valueChanged(int value);

    void on_button_shift_up_clicked();

    void on_button_shift_left_clicked();

    void on_button_shift_down_clicked();

    void on_button_shift_right_clicked();

    void on_load_button_clicked();

private:
    Ui::EditorWindow *ui;
    Map *_map;

    ListEditorWidget *_tileList;
    ListEditorWidget *_flagList;
    LabelEditorWidget *_gridPosLabel;

    QPoint lastClickedTile;
    TileFlag selectedFlag;
    TileType selectedTileType;

    void updateUI();

    void operateOnTile(QPoint tile);

    TileType tileTypeForListRow[N_TILETYPES];


    QRect toolArea();


    QRect mapArea();


    EditingMode mode;
};

#endif // EDITORWINDOW_H
