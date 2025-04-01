#include "mainwindow.h"

#include <QFile>
#include <QApplication>

int main(int argc, char *argv[])
{
    // QApplication是 Qt 应用程序的基础，它管理应用程序的全局设置和事件循环
    QApplication a(argc, argv);

    // 使用 QFile 类加载外部 QSS 文件（用于自定义应用程序的样式）
    QFile qss(":/style/styleSheet.qss");

    // 判断 QSS 文件是否成功打开
    if (qss.open(QFile::ReadOnly))
    {
        qDebug("QSS 打开成功");  // 如果打开成功，输出调试信息

        // 读取 QSS 文件内容并转换为 QString
        QString style = QLatin1String(qss.readAll());

        // 将读取到的样式设置为应用程序的样式表
        a.setStyleSheet(style);

        // 关闭 QSS 文件
        qss.close();
    }
    else
    {
        qDebug("QSS 打开失败");  // 如果打开失败，输出调试信息
    }

    // 创建并显示主窗口 MainWindow
    MainWindow w;
    w.show();

    // 进入应用程序的事件循环并返回程序退出状态
    return a.exec();
}
