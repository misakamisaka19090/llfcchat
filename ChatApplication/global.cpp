#include "global.h"

QString gate_url_prefix = "";

// 定义了一个可调用对象类型的变量 repolish，它接收一个 QWidget* 作为参数，并且返回值为 void
std::function<void(QWidget*)> repolish = [](QWidget *w) {
    w->style()->unpolish(w); // 取消应用的样式
    w->style()->polish(w);   // 重新应用样式
};
