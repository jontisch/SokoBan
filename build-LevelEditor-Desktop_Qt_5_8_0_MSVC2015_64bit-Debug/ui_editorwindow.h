/********************************************************************************
** Form generated from reading UI file 'editorwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
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
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
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
        list_tile_types->setGeometry(QRect(840, 50, 131, 192));
        save_button = new QPushButton(centralWidget);
        save_button->setObjectName(QStringLiteral("save_button"));
        save_button->setGeometry(QRect(860, 270, 80, 21));
        list_flags = new QListWidget(centralWidget);
        list_flags->setObjectName(QStringLiteral("list_flags"));
        list_flags->setGeometry(QRect(980, 50, 111, 191));
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
        EditorWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(EditorWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        EditorWindow->setStatusBar(statusBar);

        retranslateUi(EditorWindow);

        QMetaObject::connectSlotsByName(EditorWindow);
    } // setupUi

    void retranslateUi(QMainWindow *EditorWindow)
    {
        EditorWindow->setWindowTitle(QApplication::translate("EditorWindow", "EditorWindow", Q_NULLPTR));
        save_button->setText(QApplication::translate("EditorWindow", "Save map", Q_NULLPTR));
        radio_add_flag->setText(QApplication::translate("EditorWindow", "Add flag", Q_NULLPTR));
        radio_remove_flag->setText(QApplication::translate("EditorWindow", "Remove flag", Q_NULLPTR));
        radio_set_type->setText(QApplication::translate("EditorWindow", "Set type", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class EditorWindow: public Ui_EditorWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITORWINDOW_H
