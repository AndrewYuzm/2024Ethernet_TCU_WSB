/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.4.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCharts/QChartView>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QChartView *centralwidget;
    QChartView *mainChart;
    QLabel *label;
    QLineEdit *IP_lineEdit;
    QLabel *label_2;
    QLineEdit *PortlineEdit_2;
    QPushButton *connectButton;
    QPushButton *Disc_pushButton;
    QMenuBar *menubar;
    QMenu *menuTCPTest;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1003, 600);
        centralwidget = new QChartView(MainWindow);
        centralwidget->setObjectName("centralwidget");
        mainChart = new QChartView(centralwidget);
        mainChart->setObjectName("mainChart");
        mainChart->setGeometry(QRect(210, 10, 581, 401));
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(20, 20, 54, 16));
        IP_lineEdit = new QLineEdit(centralwidget);
        IP_lineEdit->setObjectName("IP_lineEdit");
        IP_lineEdit->setGeometry(QRect(20, 40, 171, 23));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(20, 80, 54, 16));
        PortlineEdit_2 = new QLineEdit(centralwidget);
        PortlineEdit_2->setObjectName("PortlineEdit_2");
        PortlineEdit_2->setGeometry(QRect(20, 100, 171, 23));
        connectButton = new QPushButton(centralwidget);
        connectButton->setObjectName("connectButton");
        connectButton->setGeometry(QRect(20, 140, 80, 24));
        Disc_pushButton = new QPushButton(centralwidget);
        Disc_pushButton->setObjectName("Disc_pushButton");
        Disc_pushButton->setGeometry(QRect(110, 140, 80, 24));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1003, 21));
        menuTCPTest = new QMenu(menubar);
        menuTCPTest->setObjectName("menuTCPTest");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuTCPTest->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Host IP:", nullptr));
        IP_lineEdit->setText(QCoreApplication::translate("MainWindow", "192.168.56.1", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Port:", nullptr));
        PortlineEdit_2->setText(QCoreApplication::translate("MainWindow", "5001", nullptr));
        connectButton->setText(QCoreApplication::translate("MainWindow", "Connect", nullptr));
        Disc_pushButton->setText(QCoreApplication::translate("MainWindow", "Disconnect", nullptr));
        menuTCPTest->setTitle(QCoreApplication::translate("MainWindow", "TCPTest", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
