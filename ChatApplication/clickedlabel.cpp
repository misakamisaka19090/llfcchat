#include "clickedlabel.h"
#include <QMouseEvent>

/**
 * @brief ClickedLabel 构造函数
 * @param parent 父控件指针
 *
 * @details
 * - 初始化状态为 Normal
 * - 设置光标形状为手型指针
 */
ClickedLabel::ClickedLabel(QWidget* parent)
    : QLabel(parent), _curstate(ClickLbState::Normal)
{
    this->setCursor(Qt::PointingHandCursor); // 设置交互光标
}

/**
 * @brief 设置六种状态图片路径
 * @param normal        普通状态默认
 * @param hover         普通悬停
 * @param press         普通按下（当前实现未使用）
 * @param select        选中状态默认
 * @param select_hover  选中悬停
 * @param select_press  选中按下（当前实现未使用）
 *
 * @warning
 * - 空字符串参数将覆盖原有路径
 * - 建议：添加路径有效性检查
 * - 待优化：支持资源文件前缀自动补全
 */
void ClickedLabel::SetState(QString normal, QString hover, QString press,
    QString select, QString select_hover, QString select_press)
{
    _normal = normal;
    _normal_hover = hover;
    _normal_press = press;           // 注意：当前逻辑未使用按压状态
    _selected = select;
    _selected_hover = select_hover;
    _selected_press = select_press;  // 注意：当前逻辑未使用按压状态

    setProperty("state", normal);    // 初始化状态
    repolish(this);                  // 应用新样式
}

/**
    * @brief 获取当前控件状态
        * @return ClickLbState 枚举值
    *
    * @note 可用于外部状态判断，例如：
    * if(label->GetCurState() == ClickLbState::Selected)
    */
ClickLbState ClickedLabel::GetCurState()
{
    return _curstate;
}

/**
 * @brief 设置当前控件状态
 * @param state 当前控件状态
 * @return bool 设置成功与否
 *
 * @details 该函数根据传入的状态设置控件的样式。
 */
bool ClickedLabel::SetCurState(ClickLbState state)
{
    _curstate = state;
    if (_curstate == ClickLbState::Normal) {
        setProperty("state", _normal);
    }
    else if (_curstate == ClickLbState::Selected) {
        setProperty("state", _selected);
    }
    repolish(this);
    return true;
}


/**
 * @brief 重置为普通状态
 *
 * @details 此方法将控件恢复到普通状态，适用于外部控制。
 */
void ClickedLabel::ResetNormalState()
{
    _curstate = ClickLbState::Normal;
    setProperty("state", _normal);
    repolish(this);
}

/**
 * @brief 处理鼠标按下事件
 * @param event 鼠标事件对象
 *
 * @details
 * - 仅响应左键点击
 * - 切换 Normal/Selected 状态
 * - 更新控件样式
 * - 发射 clicked() 信号
 *
 * @warning 必须调用基类实现以保证事件传递链完整
 */
void ClickedLabel::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        // 状态切换逻辑
        if(_curstate == ClickLbState::Normal){
            qDebug() << "[State] Switch to Selected (hover): " << _selected_hover;
            _curstate = ClickLbState::Selected;
            setProperty("state", _selected_hover); // 设置QSS属性
        } else {
            qDebug() << "[State] Switch to Normal (hover): " << _normal_hover;
            _curstate = ClickLbState::Normal;
            setProperty("state", _normal_hover);
        }

        repolish(this); // 强制刷新样式（假设为自定义样式更新方法）
        update();       // 触发重绘

        //emit clicked(); // 发射点击信号
    }
    QLabel::mousePressEvent(event); // 关键：基类事件处理
}

/**
 * @brief 处理鼠标释放事件
 * @param event 鼠标事件对象
 *
 * @details
 * - 仅响应左键释放
 * - 根据当前状态切换至对应悬停样式
 * - 发射 clicked() 信号，传递标签文本和当前状态
 */
void ClickedLabel::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        // 根据当前状态选择悬停图片
        if (_curstate == ClickLbState::Normal) {
            qDebug() << "[Release] Change to Normal Hover: " << _normal_hover;
            setProperty("state", _normal_hover);
        }
        else {
            qDebug() << "[Release] Change to Selected Hover: " << _selected_hover;
            setProperty("state", _selected_hover);
        }

        repolish(this); // 强制刷新样式
        update();       // 触发重绘

        emit clicked(this->text(), _curstate); // 发射带有状态的点击信号
    }
    QLabel::mousePressEvent(event); // 关键：基类事件处理
}

/**
 * @brief 处理鼠标进入事件
 * @param event 进入事件对象
 *
 * @details
 * - 根据当前状态切换至对应悬停样式
 * - 调试输出状态变更信息
 * - 建议：可添加图片预加载优化性能
 */
void ClickedLabel::enterEvent(QEnterEvent* event)
{
    if(_curstate == ClickLbState::Normal) {
        qDebug() << "[Hover] Normal → Hover: " << _normal_hover;
        setProperty("state", _normal_hover);
    } else {
        qDebug() << "[Hover] Selected → Hover: " << _selected_hover;
        setProperty("state", _selected_hover);
    }
    repolish(this);
    update();
    QLabel::enterEvent(event); // 传递事件
}

/**
 * @brief 处理鼠标离开事件
 * @param event 离开事件对象
 *
 * @details
 * - 返回基础状态样式（Normal/Selected）
 * - 注意状态恢复逻辑的对称性
 * - 典型问题：快速划过可能导致状态残留
 */
void ClickedLabel::leaveEvent(QEvent* event)
{
    if(_curstate == ClickLbState::Normal) {
        qDebug() << "[Leave] Restore Normal: " << _normal;
        setProperty("state", _normal);
    } else {
        qDebug() << "[Leave] Restore Selected: " << _selected;
        setProperty("state", _selected);
    }
    repolish(this);
    update();
    QLabel::leaveEvent(event); // 传递事件
}