/********************************************************************************
** Form generated from reading UI file 'editorwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDITORWINDOW_H
#define UI_EDITORWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EditorWindow
{
public:
    QWidget *centralWidget;
    QListWidget *list_tile_types;
    QPushButton *save_button;
    QListWidget *list_flags;
    QRadioButton *radio_add_flag;
    QRadioButton *radio_remove_flag;
    QRadioButton *radio_set_type;
    QSlider *slider_width;
    QSlider *slider_height;
    QPushButton *button_update_size;
    QLabel *label_width;
    QLabel *label_height;
    QPushButton *button_shift_up;
    QPushButton *button_shift_down;
    QPushButton *button_shift_right;
    QPushButton *button_shift_left;
    QPushButton *load_button;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *EditorWindow)
    {
        if (EditorWindow->objectName().isEmpty())
            EditorWindow->setObjectName(QStringLiteral("EditorWindow"));
        EditorWindow->resize(1109, 631);
        centralWidget = new QWidget(EditorWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        list_tile_types = new QListWidget(centralWidget);
        list_tile_types->setObjectName(QStringLiteral("list_tile_types"));
        list_tile_types->setGeometry(QRect(880, 560, 20, 20));
        save_button = new QPushButton(centralWidget);
        save_button->setObjectName(QStringLiteral("save_button"));
        save_button->setGeometry(QRect(860, 270, 80, 21));
        list_flags = new QListWidget(centralWidget);
        list_flags->setObjectName(QStringLiteral("list_flags"));
        list_flags->setGeometry(QRect(1050, 560, 20, 20));
        radio_add_flag = new QRadioButton(centralWidget);
        radio_add_flag->setObjectName(QStringLiteral("radio_add_flag"));
        radio_add_flag->setGeometry(QRect(940, 20, 82, 17));
        radio_remove_flag = new QRadioButton(centralWidget);
        radio_remove_flag->setObjectName(QStringLiteral("radio_remove_flag"));
        radio_remove_flag->setGeometry(QRect(1010, 20, 82, 17));
        radio_set_type = new QRadioButton(centralWidget);
        radio_set_type->setObjectName(QStringLiteral("radio_set_type"));
        radio_set_type->setGeometry(QRect(850, 20, 82, 17));
        radio_set_type->setChecked(true);
        slider_width = new QSlider(centralWidget);
        slider_width->setObjectName(QStringLiteral("slider_width"));
        slider_width->setGeometry(QRect(900, 310, 160, 16));
        slider_width->setOrientation(Qt::Horizontal);
        slider_height = new QSlider(centralWidget);
        slider_height->setObjectName(QStringLiteral("slider_height"));
        slider_height->setGeometry(QRect(870, 310, 16, 160));
        slider_height->setOrientation(Qt::Vertical);
        button_update_size = new QPushButton(centralWidget);
        button_update_size->setObjectName(QStringLiteral("button_update_size"));
        button_update_size->setGeometry(QRect(970, 380, 80, 21));
        label_width = new QLabel(centralWidget);
        label_width->setObjectName(QStringLiteral("label_width"));
        label_width->setGeometry(QRect(970, 330, 47, 13));
        label_height = new QLabel(centralWidget);
        label_height->setObjectName(QStringLiteral("label_height"));
        label_height->setGeometry(QRect(900, 370, 47, 13));
        button_shift_up = new QPushButton(centralWidget);
        button_shift_up->setObjectName(QStringLiteral("button_shift_up"));
        button_shift_up->setGeometry(QRect(940, 480, 41, 31));
        button_shift_down = new QPushButton(centralWidget);
        button_shift_down->setObjectName(QStringLiteral("button_shift_down"));
        button_shift_down->setGeometry(QRect(940, 540, 41, 31));
        button_shift_right = new QPushButton(centralWidget);
        button_shift_right->setObjectName(QStringLiteral("button_shift_right"));
        button_shift_right->setGeometry(QRect(980, 510, 41, 31));
        button_shift_left = new QPushButton(centralWidget);
        button_shift_left->setObjectName(QStringLiteral("button_shift_left"));
        button_shift_left->setGeometry(QRect(900, 510, 41, 31));
        load_button = new QPushButton(centralWidget);
        load_button->setObjectName(QStringLiteral("load_button"));
        load_button->setGeometry(QRect(990, 270, 80, 21));
        EditorWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(EditorWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        EditorWindow->setStatusBar(statusBar);

        retranslateUi(EditorWindow);

        QMetaObject::connectSlotsByName(EditorWindow);
    } // setupUi

    void retranslateUi(QMainWindow *EditorWindow)
    {
        EditorWindow->setWindowTitle(QApplication::translate("EditorWindow", "EditorWindow", 0));
        save_button->setText(QApplication::translate("EditorWindow", "Save map", 0));
        radio_add_flag->setText(QApplication::translate("EditorWindow", "Add flag", 0));
        radio_remove_flag->setText(QApplication::translate("EditorWindow", "Remove flag", 0));
        radio_set_type->setText(QApplication::translate("EditorWindow", "Set type", 0));
        button_update_size->setText(QApplication::translate("EditorWindow", "Update size", 0));
        label_width->setText(QApplication::translate("EditorWindow", "0", 0));
        label_height->setText(QApplication::translate("EditorWindow", "0", 0));
        button_shift_up->setText(QApplication::translate("EditorWindow", "Up", 0));
        button_shift_down->setText(QApplication::translate("EditorWindow", "Down", 0));
        button_shift_right->setText(QApplication::translate("EditorWindow", "Right", 0));
        button_shift_left->setText(QApplication::translate("EditorWindow", "Left", 0));
        load_button->setText(QApplication::translate("EditorWindow", "Load map", 0));
    } // retranslateUi

};

namespace Ui {
    class EditorWindow: public Ui_EditorWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITORWINDOW_H
