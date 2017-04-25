#include "editor.h"
//#include <QApplication>
#include <QListWidgetItem>
#include <QDesktopWidget>
#include <QDebug>
#include <QFileDialog>
#include <math.h>
#include "../map/entities/door.h"

#include <QGraphicsItem>

#define TOOL_AREA_WIDTH 240

Editor::Editor()
{
    _map = new Map(24,16,11,7);


    //HMM
    _toolboxRadioCluster = new RadioClusterEditorWidget("Tools",0);
    _toolboxRadioCluster->addRadio(new RadioEditorWidget("Draw", true));
    _toolboxRadioCluster->addRadio(new RadioEditorWidget("Misc", false));


    _drawToolBox = new ToolboxEditorWidget("Draw", QSize(12,15));
    _dimensionsToolBox = new ToolboxEditorWidget("Dimensions", QSize(12,15));

    _visibleToolBox = _drawToolBox;


    _tileList = new ListEditorWidget("Tiles",QSize(6,7), QPoint(0,0));
    _flagList = new ListEditorWidget("Flags", QSize(6,7), QPoint(6,0));
    _entityList = new ListEditorWidget("Entities", QSize(12,3),QPoint(0,7));

    _mapPosLabel = new LabelEditorWidget("Position", "X:0\tY:0", QSize(12,1), QPoint(0,14));


    _tileRadioCluster = new RadioClusterEditorWidget("Tiles", 0, QSize(12,1), QPoint(0,10));
    RadioEditorWidget *_setTypeRadio = new RadioEditorWidget("Type", true);
    RadioEditorWidget *_fillTypeRadio = new RadioEditorWidget("Fill", false);
    RadioEditorWidget *_addFlagRadio = new RadioEditorWidget("+Flag", false);
    RadioEditorWidget *_removeFlagRadio = new RadioEditorWidget("-Flag", false);
    RadioEditorWidget *_addEntityRadio = new RadioEditorWidget("+Ent", false);
    RadioEditorWidget *_removeEntityRadio = new RadioEditorWidget("-Ent", false);
    _tileRadioCluster->addRadio(_setTypeRadio);
    _tileRadioCluster->addRadio(_fillTypeRadio);
    _tileRadioCluster->addRadio(_addFlagRadio);
    _tileRadioCluster->addRadio(_removeFlagRadio);
    _tileRadioCluster->addRadio(_addEntityRadio);
    _tileRadioCluster->addRadio(_removeEntityRadio);

    _colorRadioCluster = new RadioClusterEditorWidget("Colors", 0, QSize(12,1), QPoint(0,11));
    RadioEditorWidget *_blueColorRadio = new RadioEditorWidget("Blue", true);
    RadioEditorWidget *_redColorRadio = new RadioEditorWidget("Red", false);
    RadioEditorWidget *_greenColorRadio = new RadioEditorWidget("Green", false);
    _colorRadioCluster->addRadio(_blueColorRadio);
    _colorRadioCluster->addRadio(_redColorRadio);
    _colorRadioCluster->addRadio(_greenColorRadio);


    _mapNameTextfield = new TextfieldEditorWidget("Map name", "No name", QSize(12, 1));

    _shiftUpButton = new ButtonEditorWidget("Up", true, QSize(2,1), QPoint(5,6));
    _shiftDownButton = new ButtonEditorWidget("Down", true, QSize(2,1), QPoint(5,8));
    _shiftLeftButton = new ButtonEditorWidget("Left", true, QSize(2,1), QPoint(3,7));
    _shiftRightButton = new ButtonEditorWidget("Right", true, QSize(2,1), QPoint(7,7));
    _shiftLabel = new LabelEditorWidget("Shift", NULL, QSize(2,1), QPoint(5,7));

    _widthTextfield = new TextfieldEditorWidget("Width", "0", QSize(6,1), QPoint(0,10), true);
    _heightTextfield = new TextfieldEditorWidget("Height", "0", QSize(6,1), QPoint(6,10), true);

    _updateMapButton = new ButtonEditorWidget("Update Map", true, QSize(12,1), QPoint(0,12));

    _saveMapButton = new ButtonEditorWidget("Save Map", true, QSize(6,2), QPoint(0,2));
    _loadMapButton = new ButtonEditorWidget("Load Map", true, QSize(6,2), QPoint(6,2));

    _activeTextField = NULL;
    _hoveredWidget = NULL;



    //Tools added to the Drawing toolbox
    _drawToolBox->addWidget(_tileList);
    _drawToolBox->addWidget(_flagList);
    _drawToolBox->addWidget(_entityList);
    _drawToolBox->addWidget(_mapPosLabel);
    _drawToolBox->addWidget(_tileRadioCluster);
    _drawToolBox->addWidget(_colorRadioCluster);



    //Tools added to the Dimensions toolbox

    _dimensionsToolBox->addWidget(_mapNameTextfield);
    _dimensionsToolBox->addWidget(_loadMapButton);
    _dimensionsToolBox->addWidget(_saveMapButton);
    _dimensionsToolBox->addWidget(_shiftUpButton);
    _dimensionsToolBox->addWidget(_shiftDownButton);
    _dimensionsToolBox->addWidget(_shiftLeftButton);
    _dimensionsToolBox->addWidget(_shiftRightButton);
    _dimensionsToolBox->addWidget(_shiftLabel);
    _dimensionsToolBox->addWidget(_widthTextfield);
    _dimensionsToolBox->addWidget(_heightTextfield);
    _dimensionsToolBox->addWidget(_updateMapButton);
    _dimensionsToolBox->addWidget(_mapPosLabel);


    //Gradient settings
    _buttonGradient.setColorAt(0,QColor(59,148,204));
    _buttonGradient.setColorAt(1,QColor(28,73,102));



    //END OF HMM
    selectedFlag = HAS_BOX;
    selectedTileType = FLOOR;
    selectedEntityType = DOOR_VERTICAL;

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

    for(int i = 0; i < N_ENTITIES; i++){
        _entityList->addItem(new QString(EntityTypeName((EntityType)i)), Pixmap(PixmapForEntity((EntityType)i)));

    }

    _mode = SET_TYPE;
    _entityColor = ENTITY_COLOR_BLUE;
    updateUI();
}

QRect Editor::mapArea(QRect renderRect)
{
    return QRect(renderRect.x(), renderRect.y(), renderRect.width() - TOOL_AREA_WIDTH, renderRect.height());
}


QRect Editor::toolArea(QRect renderRect)
{
   QRect mapArea = this->mapArea(renderRect);
   return QRect(mapArea.right(), renderRect.y(), renderRect.width() - mapArea.width(), renderRect.height());
}


void Editor::operateOnTile(QPoint tile)
{
    switch(_mode)
    {
    case SET_TYPE:
        _map->setTile(tile.x(), tile.y(), selectedTileType);
        break;
    case FILL_TYPE:
        _map->fillType(tile.x(), tile.y(), selectedTileType);
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
    case ADD_ENTITY:
        _map->addEntity(tile.x(), tile.y(), selectedEntityType, _entityColor);
        break;
    case REMOVE_ENTITY:
        _map->removeEntity(tile.x(), tile.y());
        break;

    }
}

void Editor::mousePress(QMouseEvent *event, QRect renderRect, QWidget *parentWidget)
{
    QRect mapArea = this->mapArea(renderRect);

    if(event->buttons() & Qt::LeftButton)
    {
        if(_activeTextField)
        {
            _activeTextField->setEdit(false);
            _activeTextField = NULL;
        }
        QPoint mousePosition = event->pos();

        if(mapArea.contains(mousePosition)){
            QPoint tile = _map->pixelToTile(mousePosition.x(), mousePosition.y(), mapArea);
            operateOnTile(tile);
            lastClickedTile = tile;
        }

        else if(_toolboxRadioCluster->getArea()->contains(mousePosition)){
            int sel = _toolboxRadioCluster->select(mousePosition.x() - _toolboxRadioCluster->getArea()->left(), mousePosition.y() - _toolboxRadioCluster->getArea()->top());
            if(sel == 0){
                _visibleToolBox = _drawToolBox;
            } else{
                _visibleToolBox = _dimensionsToolBox;
            }
        }

        if(_visibleToolBox == _drawToolBox){

            //QPoint localMousePosition = mousePosition - renderRect.topLeft();

            if(_tileList->getArea()->contains(mousePosition)){
                _tileList->select(mousePosition.y());
                selectedTileType = tileTypeForListRow[_tileList->getSelected()];
                if(_mode != FILL_TYPE)
                    _mode = (EditingMode)_tileRadioCluster->select(0,0,0);
            }
            else if(_flagList->getArea()->contains(mousePosition)){
                _flagList->select(mousePosition.y());
                selectedFlag = (TileFlag)(int)pow(2, _flagList->getSelected());
                if(_mode != REMOVE_FLAG)
                    _mode = (EditingMode)_tileRadioCluster->select(0,0,2);

            }
            else if(_entityList->getArea()->contains(mousePosition)){
                _entityList->select(mousePosition.y());
                selectedEntityType = (EntityType)_entityList->getSelected();
                if(_mode != REMOVE_ENTITY)
                    _mode = (EditingMode)_tileRadioCluster->select(0,0,4);

            }
            else if(_tileRadioCluster->getArea()->contains(mousePosition)){
                _mode = (EditingMode)_tileRadioCluster->select(mousePosition.x() - _tileRadioCluster->getArea()->left(), mousePosition.y() - _tileRadioCluster->getArea()->top());
            }
            else if(_colorRadioCluster->getArea()->contains(mousePosition)){
                _entityColor = (EntityColor)(_colorRadioCluster->select(mousePosition.x() - _colorRadioCluster->getArea()->left(), mousePosition.y() - _colorRadioCluster->getArea()->top())+1);
            }
        }

        else if (_visibleToolBox == _dimensionsToolBox){

            if(_saveMapButton->getArea()->contains(mousePosition) && _saveMapButton->active()){
                _map->setName(_mapNameTextfield->getText());
                QString filename = QFileDialog::getSaveFileName(parentWidget,
                                                                "Save map",
                                                                LocalFilename("custom_maps"),
                                                                "Friendly Map Language Files (*.fml)");
                _map->saveMap(filename);
            }
            else if(_loadMapButton->getArea()->contains(mousePosition) && _loadMapButton->active()){
                QString filename = QFileDialog::getOpenFileName(parentWidget,
                                                                "Load map",
                                                                LocalFilename("custom_maps"),
                                                                "Friendly Map Language Files (*.fml)");
                if(filename == NULL) return;

                if(_map) delete _map;
                _map = new Map(filename);

                updateUI();
            }
            else if(_shiftUpButton->getArea()->contains(mousePosition) && _shiftUpButton->active()){
                _map->shiftTiles(UP);
            }
            else if(_shiftDownButton->getArea()->contains(mousePosition) && _shiftDownButton->active()){
                _map->shiftTiles(DOWN);
            }
            else if(_shiftLeftButton->getArea()->contains(mousePosition) && _shiftLeftButton->active()){
                _map->shiftTiles(LEFT);
            }
            else if(_shiftRightButton->getArea()->contains(mousePosition) && _shiftRightButton->active()){
                _map->shiftTiles(RIGHT);
            }
            else if(_updateMapButton->getArea()->contains(mousePosition) && _updateMapButton->active()){
                int w =_widthTextfield->getText().toInt();
                int h =_heightTextfield->getText().toInt();
                _map->setSize(QSize((w>0)?w:1,(h>0)?h:1));
            }
            else if(_mapNameTextfield->getArea()->contains(mousePosition)){
                _activeTextField = _mapNameTextfield;
                _activeTextField->setEdit(true);
            }
            else if(_widthTextfield->getArea()->contains(mousePosition)){
                _activeTextField = _widthTextfield;
                _activeTextField->setEdit(true);
            }
            else if(_heightTextfield->getArea()->contains(mousePosition)){
                _activeTextField = _heightTextfield;
                _activeTextField->setEdit(true);
            }
        }
    }
}


void Editor::mouseMove(QMouseEvent *Event, QRect renderRect)
{
    QRect mapArea = this->mapArea(renderRect);

    QPoint mousePosition = Event->pos();
    QPoint tile = _map->pixelToTile(mousePosition.x(), mousePosition.y(), mapArea);
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
    _mapPosLabel->setText(QString("X:" + QString::number(tile.x()+1) + "\tY:" + QString::number(tile.y()+1)));

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
    else if(_mapNameTextfield->getArea()->contains(mousePosition))
    {
        _hoveredWidget = _mapNameTextfield;
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


}

void Editor::keyPress(QKeyEvent *Event)
{
    if(_activeTextField == NULL) return;

    int key = Event->key();
    QChar character((char)key);
    if((character.isLetterOrNumber() || character.isPunct() || character == ' ') && key != Qt::Key_Shift)
    {
        _activeTextField->addChar((char)key);
    }

    switch(Event->key()){
    case Qt::Key_Backspace:
        _activeTextField->backSpace();
        break;
    default:
        break;
    }
}

void Editor::draw(QPainter *painter, QRect renderRect)
{
    QRect mapArea = this->mapArea(renderRect);
    QRect toolArea = this->toolArea(renderRect);
    painter->fillRect(renderRect, Qt::black);
    _map->draw(painter, mapArea);
    _toolboxRadioCluster->renderWidget(painter, QRect(toolArea.topLeft(), QPoint(toolArea.right(), toolArea.top()+49)), &_buttonGradient);


    _visibleToolBox->renderWidget(painter, toolArea.adjusted(0,55,0,0), &_buttonGradient);

}

Editor::~Editor()
{
    delete _map;
    delete _toolboxRadioCluster;
    delete _drawToolBox;
    delete _dimensionsToolBox;
    delete _tileList;
    delete _flagList;
    delete _entityList;
    delete _mapPosLabel;
    delete _tileRadioCluster;
    delete _colorRadioCluster;
    delete _mapNameTextfield;
    delete _shiftUpButton;
    delete _shiftDownButton;
    delete _shiftLeftButton;
    delete _shiftRightButton;
    delete _shiftLabel;
    delete _widthTextfield;
    delete _heightTextfield;
    delete _updateMapButton;
    delete _saveMapButton;
    delete _loadMapButton;
}

void Editor::updateUI()
{
    _widthTextfield->setText(QString::number(_map->width()));
    _heightTextfield->setText(QString::number(_map->height()));
    _mapNameTextfield->setText(_map->name());
}
