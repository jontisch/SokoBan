#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include "../map/map.h"
#include "../map/tile.h"
#include <QMainWindow>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QListWidgetItem>
#include "editorwidget.h"

enum EditingMode
{
    SET_TYPE = 0,
    FILL_TYPE = 1,
    ADD_FLAG = 2,
    REMOVE_FLAG = 3,
    ADD_ENTITY = 4,
    REMOVE_ENTITY = 5
};


class Editor
{

public:
    explicit Editor();
    ~Editor();

    void mouseMove(QMouseEvent *event, QRect renderRect);
    void keyPress(QKeyEvent *event);
    void draw(QPainter *painter, QRect renderRect);

    void mousePress(QMouseEvent *event, QRect renderRect, QWidget *parentWidget);

private:
    Map *_map;

    RadioClusterEditorWidget *_toolboxRadioCluster;


    ToolboxEditorWidget *_drawToolBox;
    ToolboxEditorWidget *_dimensionsToolBox;
    ToolboxEditorWidget *_visibleToolBox;

    ListEditorWidget *_tileList;
    ListEditorWidget *_flagList;
    ListEditorWidget *_entityList;

    LabelEditorWidget *_mapPosLabel;
    RadioClusterEditorWidget *_tileRadioCluster;
    RadioClusterEditorWidget *_colorRadioCluster;
    ButtonEditorWidget *_shiftUpButton;
    ButtonEditorWidget *_shiftDownButton;
    ButtonEditorWidget *_shiftLeftButton;
    ButtonEditorWidget *_shiftRightButton;
    LabelEditorWidget *_shiftLabel;
    TextfieldEditorWidget *_mapNameTextfield;
    ButtonEditorWidget *_saveMapButton;
    ButtonEditorWidget *_loadMapButton;
    ButtonEditorWidget *_updateMapButton;
    TextfieldEditorWidget *_widthTextfield;
    TextfieldEditorWidget *_heightTextfield;

    QLinearGradient _buttonGradient;


    QPoint lastClickedTile;
    QPoint firstCorner;
    TileFlag selectedFlag;
    TileType selectedTileType;
    EntityType selectedEntityType;

    void updateUI();

    void operateOnTile(QPoint tile);

    TileType tileTypeForListRow[N_TILETYPES];


    QRect toolArea(QRect renderRect);
    QRect mapArea(QRect renderRect);

    TextfieldEditorWidget *_activeTextField;
    EditorWidget *_hoveredWidget;

    EditingMode _mode;
    EntityColor _entityColor;

};

#endif // EDITORWINDOW_H
