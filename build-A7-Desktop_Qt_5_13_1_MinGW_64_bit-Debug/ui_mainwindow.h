/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.13.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *leftPanel;
    QPushButton *eraserButton;
    QPushButton *pencilButton;
    QLabel *rightPanel;
    QPushButton *secondaryBrushButton;
    QPushButton *primaryBrushButton;
    QPushButton *resetBrushesButton;
    QPushButton *swapBrushesButton;
    QLabel *background;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1280, 720);
        MainWindow->setMouseTracking(true);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        leftPanel = new QLabel(centralwidget);
        leftPanel->setObjectName(QString::fromUtf8("leftPanel"));
        leftPanel->setGeometry(QRect(0, 0, 150, 720));
        leftPanel->setAutoFillBackground(true);
        leftPanel->setFrameShape(QFrame::WinPanel);
        eraserButton = new QPushButton(centralwidget);
        eraserButton->setObjectName(QString::fromUtf8("eraserButton"));
        eraserButton->setGeometry(QRect(10, 10, 131, 28));
        pencilButton = new QPushButton(centralwidget);
        pencilButton->setObjectName(QString::fromUtf8("pencilButton"));
        pencilButton->setGeometry(QRect(10, 50, 131, 28));
        rightPanel = new QLabel(centralwidget);
        rightPanel->setObjectName(QString::fromUtf8("rightPanel"));
        rightPanel->setGeometry(QRect(1130, 0, 150, 720));
        rightPanel->setAutoFillBackground(true);
        rightPanel->setFrameShape(QFrame::WinPanel);
        secondaryBrushButton = new QPushButton(centralwidget);
        secondaryBrushButton->setObjectName(QString::fromUtf8("secondaryBrushButton"));
        secondaryBrushButton->setGeometry(QRect(60, 110, 81, 61));
        secondaryBrushButton->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        primaryBrushButton = new QPushButton(centralwidget);
        primaryBrushButton->setObjectName(QString::fromUtf8("primaryBrushButton"));
        primaryBrushButton->setGeometry(QRect(10, 90, 81, 61));
        primaryBrushButton->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        resetBrushesButton = new QPushButton(centralwidget);
        resetBrushesButton->setObjectName(QString::fromUtf8("resetBrushesButton"));
        resetBrushesButton->setGeometry(QRect(10, 150, 51, 21));
        swapBrushesButton = new QPushButton(centralwidget);
        swapBrushesButton->setObjectName(QString::fromUtf8("swapBrushesButton"));
        swapBrushesButton->setGeometry(QRect(90, 90, 51, 21));
        background = new QLabel(centralwidget);
        background->setObjectName(QString::fromUtf8("background"));
        background->setGeometry(QRect(0, 0, 1920, 1080));
        background->setPixmap(QPixmap(QString::fromUtf8("background.png")));
        MainWindow->setCentralWidget(centralwidget);
        background->raise();
        leftPanel->raise();
        eraserButton->raise();
        pencilButton->raise();
        rightPanel->raise();
        secondaryBrushButton->raise();
        primaryBrushButton->raise();
        resetBrushesButton->raise();
        swapBrushesButton->raise();
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1280, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        leftPanel->setText(QString());
        eraserButton->setText(QCoreApplication::translate("MainWindow", "Eraser", nullptr));
        pencilButton->setText(QCoreApplication::translate("MainWindow", "Pencil", nullptr));
        rightPanel->setText(QString());
        secondaryBrushButton->setText(QString());
        primaryBrushButton->setText(QString());
        resetBrushesButton->setText(QCoreApplication::translate("MainWindow", "RESET", nullptr));
        swapBrushesButton->setText(QCoreApplication::translate("MainWindow", "SWAP", nullptr));
        background->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
