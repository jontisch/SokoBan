#include "editorwindow.h"
#include "ui_editorwindow.h"
#include <QListWidgetItem>
#include <QDebug>
#include <QFileDialog>

EditorWindow::EditorWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditorWindow)
{
    ui->setupUi(this);
    initPixmaps();
    map = new Map(":/maps/map.fml");

    for(int i = 0; i < N_TILETYPES; i++)
    {
        new QListWidgetItem(tileTypeName((Tiletype)i), ui->list_tile_types);
    }

    selectedTileType = FLOOR;

}

QRect EditorWindow::mapArea()
{
    QRect windowArea = this->rect();
    return QRect(0, 0, windowArea.width() - 240, windowArea.height());
}

void EditorWindow::mousePressEvent(QMouseEvent *Event)
{
    if(Event->buttons() & Qt::LeftButton)
    {
        qDebug() << "Left button";
        QPoint mousePosition = Event->pos();
        QPoint tile = map->pixelToTile(mousePosition.x(), mousePosition.y(), mapArea());
        map->setTile(tile.x(), tile.y(), selectedTileType);
        lastClickedTile = tile;
        this->repaint();
    }
}

void EditorWindow::mouseMoveEvent(QMouseEvent *Event)
{
    if(Event->buttons() & Qt::LeftButton)
    {
        QPoint mousePosition = Event->pos();
        QPoint tile = map->pixelToTile(mousePosition.x(), mousePosition.y(), mapArea());
        if(tile != lastClickedTile)
        {
            map->setTile(tile.x(), tile.y(), selectedTileType);
            lastClickedTile = tile;
            this->repaint();
        }
    }

}

void EditorWindow::paintEvent(QPaintEvent *Event)
{
    QPainter painter(this);
    map->draw(&painter, mapArea());
}

EditorWindow::~EditorWindow()
{
    delete ui;
}

void EditorWindow::on_list_tile_types_itemSelectionChanged()
{
    int row = ui->list_tile_types->row(ui->list_tile_types->selectedItems().at(0));
    selectedTileType = (Tiletype)row;
}

void EditorWindow::on_save_button_clicked(bool checked)
{
    QString appPath = QCoreApplication::applicationDirPath();
    QString filename = QFileDialog::getSaveFileName(this,
                                                    tr("Save map"),
                                                    appPath + "/../../maps/",
                                                    tr("Friendly Map Language Files (*.fml)"));
    map->saveMap(filename);
}
