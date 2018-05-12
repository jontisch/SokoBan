#include "textfieldeditorwidget.h"

TextfieldEditorWidget::TextfieldEditorWidget(QString title, QString text, QSize gridUnits, QPoint gridPos, bool numeric):
    EditorWidget(title, gridUnits, gridPos),
    _text(text),
    _numeric(numeric),
    _editActive(false)
{

}

void TextfieldEditorWidget::addChar(QChar c)
{
    if(_numeric && !c.isDigit()) return;
    _text.append(c);
    if(_numeric){
        if(_text.toInt()>128){
            _text = "128";
        }
        else if(_text.toInt()<0){
            _text = "0";
        }
    }
}

void TextfieldEditorWidget::backSpace()
{
    _text.truncate(_text.length()-1);
 //   if(_text.length() == 0){
//        _text = "0";
//    }
}

void TextfieldEditorWidget::setText(QString text)
{
    if(!_numeric) _text = text;
    else
    {
        _text = "";
        for(int i = 0; i < text.length(); i++) addChar(text.at(i));
    }
}



void TextfieldEditorWidget::renderWidget(QPainter *painter, QRect renderRect, QLinearGradient *gradient)
{
    _area = renderRect;
    //painter->setPen((_editActive||_hover)?Qt::black:Qt::white);
    //painter->setBrush((_editActive||_hover)?Qt::white:Qt::black);
    gradient->setStart(_area.topLeft());
    gradient->setFinalStop(_area.bottomLeft());
    painter->setPen((_hover || _editActive)?QColor(93,121,138):QColor(6,28,43));
    painter->setBrush(*gradient);
    painter->drawRoundedRect(renderRect, 10,10);
    painter->setFont(QFont(QString("sans serif"), 10, 10));
    painter->drawText(renderRect, Qt::AlignHCenter, _title);
    painter->drawText(renderRect, Qt::AlignBottom | Qt::AlignHCenter, getText());
}

void TextfieldEditorWidget::setEdit(bool edit)
{
    _editActive = edit;
}

QString TextfieldEditorWidget::getText()
{
    QString result = "";
    bool lastWasLetterOrNumber = false;
    for(int i = 0; i < _text.length(); i++)
    {
        QChar c = _text.at(i);
        if(c.isLetterOrNumber())
        {
            if(lastWasLetterOrNumber)
                c = c.toLower();
            else
                c = c.toUpper();
            lastWasLetterOrNumber = true;
        }
        else
        {
            lastWasLetterOrNumber = false;
        }

        result.append(c);
    }
    return result;
}
