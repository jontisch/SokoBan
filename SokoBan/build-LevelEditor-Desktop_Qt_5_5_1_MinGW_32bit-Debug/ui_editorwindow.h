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
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EditorWindow
{
public:
    QWidget *centralWidget;
    QListWidget *list_tile_types;
    QPushButton *save_button;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *EditorWindow)
    {
        if (EditorWindow->objectName().isEmpty())
            EditorWindow->setObjectName(QStringLiteral("EditorWindow"));
        EditorWindow->resize(727, 381);
        centralWidget = new QWidget(EditorWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        list_tile_types = new QListWidget(centralWidget);
        list_tile_types->setObjectName(QStringLiteral("list_tile_types"));
        list_tile_types->setGeometry(QRect(460, 50, 256, 192));
        save_button = new QPushButton(centralWidget);
        save_button->setObjectName(QStringLiteral("save_button"));
        save_button->setGeometry(QRect(480, 270, 80, 21));
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
    } // retranslateUi

};

namespace Ui {
    class EditorWindow: public Ui_EditorWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITORWINDOW_H
