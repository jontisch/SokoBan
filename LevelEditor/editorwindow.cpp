#include "editorwindow.h"
#include "ui_editorwindow.h"
#include <QListWidgetItem>
#include <QDebug>
#include <QFileDialog>
#include <math.h>

EditorWindow::EditorWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditorWindow)
{
    ui->setupUi(this);
    InitPixmaps();
    QString appPath = QCoreApplication::applicationDirPath();
    qDebug() << appPath;
    _map = new Map(appPath + "/../../maps/menumap.fml");

    selectedFlag = HAS_BOX;
    selectedTileType = FLOOR;

    int tile_row = -1;
    int c = 0;
    for(int i = 0; i < N_TILETYPES; i++)
    {
        if(!IsValidTileType((TileType)i)) continue;
        new QListWidgetItem(TileTypeName((TileType)i), ui->list_tile_types);
        if(i == selectedTileType) tile_row = c;
        tileTypeForListRow[c] = (TileType)i;
        c++;
    }
    ui->list_flags->setCurrentRow(tile_row);

    int f = 1;
    int flag_row = -1;
    int d = 0;
    while(d < N_TILEFLAGS)
    {
        new QListWidgetItem(TileFlagName((TileFlag)f), ui->list_flags);
        if(f == selectedFlag) flag_row = d;
        f *= 2;
        d++;
    }
    ui->list_flags->setCurrentRow(flag_row);

    mode = SET_TYPE;

    ui->slider_width->setMinimum(1);
    ui->slider_height->setMinimum(1);
    ui->slider_width->setMaximum(MAX_MAP_SIZE);
    ui->slider_height->setMaximum(MAX_MAP_SIZE);

    updateUI();
}

QRect EditorWindow::mapArea()
{
    QRect windowArea = this->rect();
    return QRect(0, 0, windowArea.width() - 240, windowArea.height());
}

void EditorWindow::operateOnTile(QPoint tile)
{
    switch(mode)
    {
    case SET_TYPE:
        _map->setTile(tile.x(), tile.y(), selectedTileType);
        break;
    case ADD_FLAG:
        if(_map->tileIsWalkable(tile.x(), tile.y()))
        {
            _map->addTileFlag(tile.x(), tile.y(), selectedFlag);
            if(selectedFlag == IS_START)
            {
                _map->setPlayerPosition(tile.x(), tile.y());
            }
        }
        break;
    case REMOVE_FLAG:
        _map->removeTileFlag(tile.x(), tile.y(), selectedFlag);
        break;
    }
    this->repaint();
}

void EditorWindow::mousePressEvent(QMouseEvent *Event)
{
    if(Event->buttons() & Qt::LeftButton)
    {
        QPoint mousePosition = Event->pos();
        QPoint tile = _map->pixelToTile(mousePosition.x(), mousePosition.y(), mapArea());
        operateOnTile(tile);
        lastClickedTile = tile;
    }
}

void EditorWindow::mouseMoveEvent(QMouseEvent *Event)
{
    if(Event->buttons() & Qt::LeftButton)
    {
        QPoint mousePosition = Event->pos();
        QPoint tile = _map->pixelToTile(mousePosition.x(), mousePosition.y(), mapArea());
        if(tile != lastClickedTile)
        {
            operateOnTile(tile);
            lastClickedTile = tile;
        }
    }

}

void EditorWindow::paintEvent(QPaintEvent *Event)
{
    QPainter painter(this);
    _map->draw(&painter, mapArea());
}

EditorWindow::~EditorWindow()
{
    delete ui;
}

void EditorWindow::on_list_tile_types_itemSelectionChanged()
{
    int row = ui->list_tile_types->row(ui->list_tile_types->selectedItems().at(0));
    selectedTileType = tileTypeForListRow[row];
}

void EditorWindow::on_save_button_clicked(bool checked)
{
    QString appPath = QCoreApplication::applicationDirPath();
    QString filename = QFileDialog::getSaveFileName(this,
                                                    tr("Save map"),
                                                    appPath + "/../../maps/",
                                                    tr("Friendly Map Language Files (*.fml)"));
    _map->saveMap(filename);
}

void EditorWindow::on_radio_set_type_clicked()
{
    mode = SET_TYPE;
}

void EditorWindow::on_radio_add_flag_clicked()
{
    mode = ADD_FLAG;
}

void EditorWindow::on_radio_remove_flag_clicked()
{
    mode = REMOVE_FLAG;
}

void EditorWindow::on_list_flags_currentRowChanged(int currentRow)
{
    selectedFlag = (TileFlag)(int)pow(2, currentRow);
}

void EditorWindow::on_list_flags_itemClicked(QListWidgetItem *item)
{
    if(mode == SET_TYPE) ui->radio_add_flag->click();
}

void EditorWindow::on_list_tile_types_itemClicked(QListWidgetItem *item)
{
    if(mode != SET_TYPE) ui->radio_set_type->click();
}

void EditorWindow::on_button_update_size_clicked()
{
    _map->setSize(QSize(ui->slider_width->value(), ui->slider_height->value()));
    repaint();
}

void EditorWindow::updateUI()
{
    ui->slider_width->setValue(_map->width());
    ui->slider_height->setValue(_map->height());
}


void EditorWindow::on_slider_height_valueChanged(int value)
{
    ui->label_height->setText(QString::number(value));
}

void EditorWindow::on_slider_width_valueChanged(int value)
{
    ui->label_width->setText(QString::number(value));
}

void EditorWindow::on_button_shift_up_clicked()
{
    _map->shiftTiles(Map::UP);
    repaint();
}

void EditorWindow::on_button_shift_left_clicked()
{
    _map->shiftTiles(Map::LEFT);
    repaint();
}

void EditorWindow::on_button_shift_down_clicked()
{
    _map->shiftTiles(Map::DOWN);
    repaint();
}

void EditorWindow::on_button_shift_right_clicked()
{
    _map->shiftTiles(Map::RIGHT);
    repaint();
}
