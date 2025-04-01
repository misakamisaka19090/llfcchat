#include "mainwindow.h"

#include <QFile>
#include <QApplication>

/*
int ma1n(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
*/
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
/*

说起第三种 流传很广的那个月球飙车那段。是我感觉最直观。最确切的
车轮扬起的尘土轨迹和尘土运动显示出与车相对的速度。
尘土沿抛物线轨迹落回地面，没有像在地球上那样受到空气阻力影响而四散。
测量尘土下落的时间可以大概计算出尘土运动所对应的重力加速度。
这些现象只能在真空和低重力环境下产生，而地球上无法构建如此大规模的低重力或真空条件。
加上场景中有人出场，我认为足以证明那是真正的载人登月。








也可以确定视频没有剪辑。 如果视频加速了：车实际速度不足以抛出尘土，
如果视频减速了。这里想了好久有点麻烦。


 * */
