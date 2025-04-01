/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
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
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(725, 653);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(70, 90, 53, 15));
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(70, 160, 53, 15));
        label_2->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(110, 250, 281, 21));
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(150, 370, 301, 16));
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(60, 40, 341, 16));
        label_6 = new QLabel(centralwidget);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(110, 410, 451, 16));
        label_7 = new QLabel(centralwidget);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(180, 480, 16, 21));
        label_8 = new QLabel(centralwidget);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(420, 480, 201, 21));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 725, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "ChatApplication", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\345\234\260\347\220\203", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\346\234\210\347\220\203", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\357\274\232\350\241\250\347\244\272\344\270\200\346\256\265\345\243\260\351\237\263\357\274\2142\344\270\252\344\270\255\351\227\264\347\251\272\347\231\275\347\232\204\350\267\235\347\246\273\345\260\261\346\230\257\345\273\266\350\277\237", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "\357\274\232\346\241\206\344\275\217\347\232\204\344\273\243\350\241\250\346\230\257\345\220\214\344\270\200\346\256\265\345\243\260\351\237\263\357\274\214\347\256\255\345\244\264\350\241\250\347\244\272\344\272\206\345\243\260\351\237\263\347\232\204\344\274\240\346\222\255", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "\350\277\231\344\270\252\346\230\257\346\227\266\351\227\264\350\275\264\357\274\214\350\241\250\347\244\272\344\270\212\345\270\235\350\247\206\350\247\222\347\232\204\345\256\236\351\231\205\347\232\204\346\227\266\351\227\264", nullptr));
        label_6->setText(QString());
        label_7->setText(QCoreApplication::translate("MainWindow", "\345\222\214", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "\350\277\231\346\240\267\344\270\255\351\227\264\351\227\264\351\232\224\344\270\215\345\220\214\347\232\204\345\217\257\344\273\245\346\230\257\345\220\214\344\270\200\345\217\245\350\257\235", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
