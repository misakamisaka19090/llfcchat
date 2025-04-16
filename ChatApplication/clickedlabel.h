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
    /**
     * @brief 构造函数
     * @param parent 父控件指针
     */
    ClickedLabel(QWidget* parent);

    // 重写鼠标事件处理函数
    virtual void mousePressEvent(QMouseEvent* ev) override;
    virtual void enterEvent(QEnterEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;

    /**
     * @brief 设置六种状态图片路径
     * @param normal        普通状态默认图片路径（默认空表示不修改）
     * @param hover         普通悬停状态图片路径
     * @param press         普通按下状态图片路径
     * @param select        选中状态默认图片路径
     * @param select_hover  选中悬停状态图片路径
     * @param select_press  选中按下状态图片路径
     */
    void SetState(QString normal = "", QString hover = "", QString press = "",
                  QString select = "", QString select_hover = "", QString select_press = "");

    /// @brief 获取当前控件状态
    ClickLbState GetCurState();

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
    /// @brief 当标签被点击时发出的信号
    void clicked(void);
};

#endif // CLICKEDLABEL_H
