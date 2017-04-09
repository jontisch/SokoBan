#include "editorwindow.h"
#include "ui_editorwindow.h"
//#include <QApplication>
#include <QListWidgetItem>
#include <QDesktopWidget>
#include <QDebug>
#include <QFileDialog>
#include <math.h>

#define WIDGET_HEIGHT 40
#define LIST_HEIGHT 300

EditorWindow::EditorWindow(QWidget *parent) :
    QMainWindow(parent)//,
    //ui(new Ui::EditorWindow)
{
    //ui->setupUi(this);
    InitPixmaps();
    QRect screenRect = QApplication::desktop()->screenGeometry();
    this->setGeometry(screenRect.width()/2 - 1024/2, screenRect.height()/2-768/2, 1024,768);

    //centralWidget()->setAttribute(Qt::WA_TransparentForMouseEvents);
    setMouseTracking(true);
    _map = new Map(LocalFilename("/../../maps/menumap.fml"));


    //HMM
    _tileList = new ListEditorWidget("Tiles");
    _flagList = new ListEditorWidget("Flags");
    _gridPosLabel = new LabelEditorWidget("Position", "X:0\tY:0");

    _tileRadioCluster = new RadioClusterEditorWidget("Tiles", 0);
    RadioEditorWidget *_setTypeRadio = new RadioEditorWidget("Set type", true);
    RadioEditorWidget *_addFlagRadio = new RadioEditorWidget("Add flag", false);
    RadioEditorWidget *_removeFlagRadio = new RadioEditorWidget("Remove flag", false);
    _tileRadioCluster->addRadio(_setTypeRadio);
    _tileRadioCluster->addRadio(_addFlagRadio);
    _tileRadioCluster->addRadio(_removeFlagRadio);

    _colorRadioCluster = new RadioClusterEditorWidget("Colors", 0);
    RadioEditorWidget *_blueColorRadio = new RadioEditorWidget("Blue", true);
    RadioEditorWidget *_redColorRadio = new RadioEditorWidget("Red", false);
    RadioEditorWidget *_greenColorRadio = new RadioEditorWidget("Green", false);
    _colorRadioCluster->addRadio(_blueColorRadio);
    _colorRadioCluster->addRadio(_redColorRadio);
    _colorRadioCluster->addRadio(_greenColorRadio);

    _shiftUpButton = new ButtonEditorWidget("Up");
    _shiftDownButton = new ButtonEditorWidget("Down");
    _shiftLeftButton = new ButtonEditorWidget("Left");
    _shiftRightButton = new ButtonEditorWidget("Right");
    _shiftLabel = new LabelEditorWidget("Shift");
    _saveMapButton = new ButtonEditorWidget("Save Map");
    _loadMapButton = new ButtonEditorWidget("Load Map");
    _updateMapButton = new ButtonEditorWidget("Update Map");

    _widthTextfield = new TextfieldEditorWidget("Width", "0");
    _heightTextfield = new TextfieldEditorWidget("Height", "0");
    _activeTextField = NULL;
    _hoveredWidget = NULL;
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

    _mode = SET_TYPE;
    _entityColor = ENTITY_COLOR_BLUE;
    updateUI();
}

QRect EditorWindow::mapArea()
{
    QRect windowArea = this->rect();
    return QRect(0, 0, windowArea.width() - 240, windowArea.height());
}

void EditorWindow::operateOnTile(QPoint tile)
{
    switch(_mode)
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
        if(_activeTextField)
        {
            _activeTextField->setEdit(false);
            _activeTextField = NULL;
        }
        QPoint mousePosition = Event->pos();
        if(mapArea().contains(mousePosition)){
            QPoint tile = _map->pixelToTile(mousePosition.x(), mousePosition.y(), mapArea());
            operateOnTile(tile);
            lastClickedTile = tile;
        }
        else if(_tileList->getArea()->contains(mousePosition)){
            _tileList->select(this->mapFromGlobal(QCursor::pos()).y());
            selectedTileType = tileTypeForListRow[_tileList->getSelected()];
            _mode = (EditingMode)_tileRadioCluster->select(0,0,0);
            this->repaint();
        }
        else if(_flagList->getArea()->contains(mousePosition)){
            _flagList->select(this->mapFromGlobal(QCursor::pos()).y());
            selectedFlag = (TileFlag)(int)pow(2, _flagList->getSelected());
            if(_mode == SET_TYPE)
                _mode = (EditingMode)_tileRadioCluster->select(0,0,1);
            this->repaint();

        }
        else if(_tileRadioCluster->getArea()->contains(mousePosition)){
            _mode = (EditingMode)_tileRadioCluster->select(mousePosition.x() - _tileRadioCluster->getArea()->left(), mousePosition.y() - _tileRadioCluster->getArea()->top());
            this->repaint();
        }
        else if(_colorRadioCluster->getArea()->contains(mousePosition)){
            _entityColor = (EntityColor)(_colorRadioCluster->select(mousePosition.x() - _colorRadioCluster->getArea()->left(), mousePosition.y() - _colorRadioCluster->getArea()->top())+1);
            this->repaint();
            qDebug() << _entityColor;
        }
        else if(_saveMapButton->getArea()->contains(mousePosition) && _saveMapButton->active()){
            QString appPath = QCoreApplication::applicationDirPath();
            QString filename = QFileDialog::getSaveFileName(this,
                                                            tr("Save map"),
                                                            appPath + "/../../maps/",
                                                            tr("Friendly Map Language Files (*.fml)"));
            _map->saveMap(filename);
        }
        else if(_loadMapButton->getArea()->contains(mousePosition) && _loadMapButton->active()){
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
        else if(_shiftUpButton->getArea()->contains(mousePosition) && _shiftUpButton->active()){
            _map->shiftTiles(UP);
            repaint();
        }
        else if(_shiftDownButton->getArea()->contains(mousePosition) && _shiftDownButton->active()){
            _map->shiftTiles(DOWN);
            repaint();
        }
        else if(_shiftLeftButton->getArea()->contains(mousePosition) && _shiftLeftButton->active()){
            _map->shiftTiles(LEFT);
            repaint();
        }
        else if(_shiftRightButton->getArea()->contains(mousePosition) && _shiftRightButton->active()){
            _map->shiftTiles(RIGHT);
            repaint();
        }
        else if(_updateMapButton->getArea()->contains(mousePosition) && _updateMapButton->active()){
            int w =_widthTextfield->getText().toInt();
            int h =_heightTextfield->getText().toInt();
            _map->setSize(QSize((w>0)?w:1,(h>0)?h:1));

            repaint();
        }
        else if(_widthTextfield->getArea()->contains(mousePosition)){
            _activeTextField = _widthTextfield;
            _activeTextField->setEdit(true);
            repaint();
        }
        else if(_heightTextfield->getArea()->contains(mousePosition)){
            _activeTextField = _heightTextfield;
            _activeTextField->setEdit(true);
            repaint();
        }
    }
}

void EditorWindow::mouseMoveEvent(QMouseEvent *Event)
{
    QPoint mousePosition = Event->pos();
    QPoint tile = _map->pixelToTile(mousePosition.x(), mousePosition.y(), mapArea());
    if(_hoveredWidget){
        _hoveredWidget->setHover(false);
        _hoveredWidget = NULL;
    }
    if(Event->buttons() & Qt::LeftButton)
    {
        if(tile != lastClickedTile)
        {
            operateOnTile(tile);
            lastClickedTile = tile;
        }
    }
    _gridPosLabel->setText(QString("X:" + QString::number(tile.x()+1) + "\tY:" + QString::number(tile.y()+1)));

    if(_saveMapButton->getArea()->contains(mousePosition) && _saveMapButton->active())
    {
        _hoveredWidget = _saveMapButton;
    }
    else if(_loadMapButton->getArea()->contains(mousePosition) && _loadMapButton->active())
    {
        _hoveredWidget = _loadMapButton;
    }
    else if(_shiftUpButton->getArea()->contains(mousePosition) && _shiftUpButton->active())
    {
        _hoveredWidget = _shiftUpButton;
    }
    else if(_shiftDownButton->getArea()->contains(mousePosition) && _shiftDownButton->active())
    {
        _hoveredWidget = _shiftDownButton;
    }
    else if(_shiftLeftButton->getArea()->contains(mousePosition) && _shiftLeftButton->active())
    {
        _hoveredWidget = _shiftLeftButton;
    }
    else if(_shiftRightButton->getArea()->contains(mousePosition) && _shiftRightButton->active())
    {
        _hoveredWidget = _shiftRightButton;
    }
    else if(_updateMapButton->getArea()->contains(mousePosition) && _updateMapButton->active())
    {
        _hoveredWidget = _updateMapButton;
    }
    else if(_widthTextfield->getArea()->contains(mousePosition))
    {
        _hoveredWidget = _widthTextfield;
    }
    else if(_heightTextfield->getArea()->contains(mousePosition))
    {
        _hoveredWidget = _heightTextfield;
    }
    if(_hoveredWidget)
        _hoveredWidget->setHover(true);
    this->repaint();


}

void EditorWindow::keyPressEvent(QKeyEvent *Event)
{
    if(_activeTextField == NULL) return;
    switch(Event->key()){
    case Qt::Key_Backspace:
        _activeTextField->backSpace();
        break;
    case Qt::Key_0:
    case Qt::Key_1:
    case Qt::Key_2:
    case Qt::Key_3:
    case Qt::Key_4:
    case Qt::Key_5:
    case Qt::Key_6:
    case Qt::Key_7:
    case Qt::Key_8:
    case Qt::Key_9:
        _activeTextField->addChar(QChar(Event->key()));
        qDebug() << QChar(Event->key());
        break;
    default:
        break;
    }
    repaint();
}

void EditorWindow::paintEvent(QPaintEvent *Event)
{
    QPainter painter(this);
    painter.fillRect(this->rect(), Qt::black);
    _map->draw(&painter, mapArea());

    _tileList->renderWidget(&painter,QRect(mapArea().right(),0,120,LIST_HEIGHT));
    _flagList->renderWidget(&painter,QRect(mapArea().right()+120,0,120,LIST_HEIGHT));
    _tileRadioCluster->renderWidget(&painter, QRect(mapArea().right(),300,240,WIDGET_HEIGHT+10));
    _colorRadioCluster->renderWidget(&painter, QRect(mapArea().right(),350, 240, WIDGET_HEIGHT+10));
    _saveMapButton->renderWidget(&painter,QRect(mapArea().right(),400,120,WIDGET_HEIGHT));
    _loadMapButton->renderWidget(&painter,QRect(mapArea().right()+120,400,120,WIDGET_HEIGHT));

    _shiftUpButton->renderWidget(&painter,QRect(mapArea().right()+100,440,WIDGET_HEIGHT,WIDGET_HEIGHT));
    _shiftLeftButton->renderWidget(&painter,QRect(mapArea().right()+60,480,WIDGET_HEIGHT,WIDGET_HEIGHT));
    _shiftLabel->renderWidget(&painter, QRect(mapArea().right()+100,480, WIDGET_HEIGHT, WIDGET_HEIGHT));
    _shiftRightButton->renderWidget(&painter,QRect(mapArea().right()+140,480,WIDGET_HEIGHT,WIDGET_HEIGHT));
    _shiftDownButton->renderWidget(&painter,QRect(mapArea().right()+100,520,WIDGET_HEIGHT,WIDGET_HEIGHT));


    _widthTextfield->renderWidget(&painter, QRect(mapArea().right(),560, 120,WIDGET_HEIGHT));
    _heightTextfield->renderWidget(&painter, QRect(mapArea().right()+120,560, 120,WIDGET_HEIGHT));
    _updateMapButton->renderWidget(&painter,QRect(mapArea().right(),600,240,WIDGET_HEIGHT));
    _gridPosLabel->renderWidget(&painter, QRect(mapArea().right(), 640,240, WIDGET_HEIGHT));
}

EditorWindow::~EditorWindow()
{
    //delete ui;
}

void EditorWindow::updateUI()
{
    _widthTextfield->setText(QString::number(_map->width()));
    _heightTextfield->setText(QString::number(_map->height()));
}
