#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "logindialog.h"
#include "registerdialog.h"

/******************************************************************************
 *
 * @file       main.cpp
 * @brief      MainWindow 类的声明文件
 *
 * @details    该文件声明了 MainWindow 类，作为应用程序的主窗口。
 *             它继承自 QMainWindow 并包含用户界面的声明和登录、注册对话框的声明。
 *
 * @author     misaka
 * @date       2025/03/27
 *****************************************************************************/

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

public slots:
    void SlotSwitchRegister(); // 槽函数：切换到注册界面

    void SlotSwitchLogin();    // 槽函数：切换到登录界面

private:
    Ui::MainWindow* ui;               // 指向用户界面对象的指针
    LoginDialog* _login_dlg;         // 指向登录对话框对象的指针
    RegisterDialog* _register_dlg;   // 指向注册对话框对象的指针
    QStackedWidget* _stackedWidget;  // QStackedWidget 用于切换不同界面
};

#endif // MAINWINDOW_H
