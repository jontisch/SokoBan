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
    SET_TYPE = 0,
    ADD_FLAG = 1,
    REMOVE_FLAG = 2,
    ADD_ENTITY = 3,
    REMOVE_ENTITY = 4
};


class EditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditorWindow(QWidget *parent = 0);
    ~EditorWindow();

    void mousePressEvent(QMouseEvent *Event);
    void mouseMoveEvent(QMouseEvent *Event);
    void keyPressEvent(QKeyEvent *Event);
    void paintEvent(QPaintEvent *Event);

private:
    Ui::EditorWindow *ui;
    Map *_map;

    RadioClusterEditorWidget *_toolboxRadioCluster;


    ToolboxEditorWidget *_drawToolBox;
    ToolboxEditorWidget *_dimensionsToolBox;
    ToolboxEditorWidget *_visibleToolBox;

    ListEditorWidget *_tileList;
    ListEditorWidget *_flagList;
    ListEditorWidget *_entityList;

    LabelEditorWidget *_gridPosLabel;
    RadioClusterEditorWidget *_tileRadioCluster;
    RadioClusterEditorWidget *_colorRadioCluster;
    ButtonEditorWidget *_shiftUpButton;
    ButtonEditorWidget *_shiftDownButton;
    ButtonEditorWidget *_shiftLeftButton;
    ButtonEditorWidget *_shiftRightButton;
    LabelEditorWidget *_shiftLabel;
    ButtonEditorWidget *_saveMapButton;
    ButtonEditorWidget *_loadMapButton;
    ButtonEditorWidget *_updateMapButton;
    TextfieldEditorWidget *_widthTextfield;
    TextfieldEditorWidget *_heightTextfield;

    QLinearGradient _buttonGradient;


    QPoint lastClickedTile;
    TileFlag selectedFlag;
    TileType selectedTileType;
    EntityType selectedEntityType;

    void updateUI();

    void operateOnTile(QPoint tile);

    TileType tileTypeForListRow[N_TILETYPES];


    QRect toolArea();


    QRect mapArea();

    TextfieldEditorWidget *_activeTextField;
    EditorWidget *_hoveredWidget;

    EditingMode _mode;
    EntityColor _entityColor;

};

#endif // EDITORWINDOW_H
