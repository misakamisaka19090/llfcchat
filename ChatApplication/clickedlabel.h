#ifndef CLICKEDLABEL_H
#define CLICKEDLABEL_H
#include <QLabel>
#include "global.h"

/**
 * @class ClickedLabel
 * @brief 支持点击事件和多种状态样式的自定义标签控件
 *
 * 继承自 QLabel，通过重写鼠标事件实现以下特性：
 * 1. 支持 clicked 信号
 * 2. 支持六种状态样式切换（普通/选中 × 常态/悬停/按下）
 * 3. 自动处理鼠标交互状态变化
 */
class ClickedLabel : public QLabel
{
    Q_OBJECT
public:
    /// @brief 构造函数
    explicit ClickedLabel(QWidget* parent = nullptr);

    /// @brief 设置六种状态图片路径
    void SetState(QString normal = "", QString hover = "", QString press = "",
        QString select = "", QString select_hover = "", QString select_press = "");

    /// @brief 获取当前状态
    ClickLbState GetCurState();

    /// @brief 设置当前状态
    bool SetCurState(ClickLbState state);

    /// @brief 重置为普通默认状态
    void ResetNormalState();

protected:
    // 鼠标事件重写
    virtual void mousePressEvent(QMouseEvent* ev) override;
    virtual void mouseReleaseEvent(QMouseEvent* ev) override;
    virtual void enterEvent(QEnterEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;

protected:
private:
    // 状态图片路径存储
    QString _normal;            // 普通状态默认图片
    QString _normal_hover;      // 普通悬停状态图片
    QString _normal_press;      // 普通按下状态图片
    QString _selected;          // 选中状态默认图片
    QString _selected_hover;    // 选中悬停状态图片
    QString _selected_press;    // 选中按下状态图片

    ClickLbState _curstate;     // 当前状态（枚举类型）

signals:
    /// @brief 标签被点击时发出，带名称和状态
    void clicked(QString name, ClickLbState state);
};

#endif // CLICKEDLABEL_H
