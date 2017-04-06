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
    qDebug() << centralWidget();
    //centralWidget()->setAttribute(Qt::WA_TransparentForMouseEvents);
    setMouseTracking(true);
    QString appPath = QCoreApplication::applicationDirPath();

    qDebug() << appPath;
    _map = new Map(appPath + "/../../maps/menumap.fml");
    //HMM
    _tileList = new ListEditorWidget("Tiles");
    _flagList = new ListEditorWidget("Flags");
    _gridPosLabel = new LabelEditorWidget("Position", "X:0\tY:0");

    _radioCluster = new RadioClusterEditorWidget("Tiles", 0);
    RadioEditorWidget *_setTypeRadio = new RadioEditorWidget("Set type", true);
    RadioEditorWidget *_addFlagRadio = new RadioEditorWidget("Add flag", false);
    RadioEditorWidget *_removeFlagRadio = new RadioEditorWidget("Remove flag", false);
    _radioCluster->addRadio(_setTypeRadio);
    _radioCluster->addRadio(_addFlagRadio);
    _radioCluster->addRadio(_removeFlagRadio);

    //END OF HMM
    selectedFlag = HAS_BOX;
    selectedTileType = FLOOR;

    int tile_row = -1;
    int c = 0;
    for(int i = 0; i < N_TILETYPES; i++)
    {
        if(!IsValidTileType((TileType)i)) continue;

        _tileList->addItem(new QString(TileTypeName((TileType)i)), Pixmap(PixmapForTileType((TileType)i)));

        if(i == selectedTileType) tile_row = c;
        tileTypeForListRow[c] = (TileType)i;
        c++;
    }

    int f = 1;
    int flag_row = -1;
    int d = 0;
    while(d < N_TILEFLAGS)
    {

        _flagList->addItem(new QString(TileFlagName((TileFlag)f)), Pixmap(PixmapForTileFlag((TileFlag)f)));

        if(f == selectedFlag) flag_row = d;
        f *= 2;
        d++;
    }

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
        if((selectedFlag != HAS_BOX && selectedFlag != IS_TARGET && selectedFlag != IS_START) || _map->tileIsWalkable(tile.x(), tile.y()))
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
        if(mapArea().contains(mousePosition)){
            QPoint tile = _map->pixelToTile(mousePosition.x(), mousePosition.y(), mapArea());
            operateOnTile(tile);
            lastClickedTile = tile;
        }

        else if(_tileList->getArea()->contains(mousePosition)){
            qDebug() << "tilelist";
            _tileList->select(this->mapFromGlobal(QCursor::pos()).y());
            selectedTileType = tileTypeForListRow[_tileList->getSelected()];
            mode = (EditingMode)_radioCluster->select(0,0,0);
            this->repaint();
        }
        else if(_flagList->getArea()->contains(mousePosition)){
            qDebug() << "flaglist";
            _flagList->select(this->mapFromGlobal(QCursor::pos()).y());
            selectedFlag = (TileFlag)(int)pow(2, _flagList->getSelected());
            mode = (EditingMode)_radioCluster->select(0,0,1);
            this->repaint();

        }
        else if(_radioCluster->getArea()->contains(mousePosition)){
            mode = (EditingMode)_radioCluster->select(mousePosition.x() - _radioCluster->getArea()->left(), mousePosition.y() - _radioCluster->getArea()->top());
            this->repaint();
        }
    }
}

void EditorWindow::mouseMoveEvent(QMouseEvent *Event)
{
    QPoint mousePosition = Event->pos();
    QPoint tile = _map->pixelToTile(mousePosition.x(), mousePosition.y(), mapArea());
    if(Event->buttons() & Qt::LeftButton)
    {
        if(tile != lastClickedTile)
        {
            operateOnTile(tile);
            lastClickedTile = tile;
        }
    }
    _gridPosLabel->setText(QString("X:" + QString::number(tile.x()) + "\tY:" + QString::number(tile.y())));
    this->repaint();


}

void EditorWindow::paintEvent(QPaintEvent *Event)
{
    QPainter painter(this);
    _map->draw(&painter, mapArea());
    _radioCluster->renderWidget(&painter, QRect(mapArea().right(),0,240,50));
    _tileList->renderWidget(&painter,QRect(mapArea().right(),50,120,350));
    _flagList->renderWidget(&painter,QRect(mapArea().right()+120,50,120,350));
    _gridPosLabel->renderWidget(&painter, QRect(mapArea().right(), 360, 290, 50));

}

EditorWindow::~EditorWindow()
{
    delete ui;
}



void EditorWindow::on_load_button_clicked()
{
    QString appPath = QCoreApplication::applicationDirPath();
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Load map"),
                                                    appPath + "/../../maps/",
                                                    tr("Friendly Map Language Files (*.fml)"));
    if(filename == NULL) return;

    if(_map) delete _map;
    _map = new Map(filename);

    updateUI();
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
    _map->shiftTiles(UP);
    repaint();
}

void EditorWindow::on_button_shift_left_clicked()
{
    _map->shiftTiles(LEFT);
    repaint();
}

void EditorWindow::on_button_shift_down_clicked()
{
    _map->shiftTiles(DOWN);
    repaint();
}

void EditorWindow::on_button_shift_right_clicked()
{
    _map->shiftTiles(RIGHT);
    repaint();
}

