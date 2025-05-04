#include "ChatItemBase.h"
#include <QFont>
#include <QVBoxLayout>
#include "BubbleFrame.h"

// ChatItemBase类构造函数，初始化聊天项
ChatItemBase::ChatItemBase(ChatRole role, QWidget* parent)
    : QWidget(parent)
    , m_role(role)  // 保存聊天角色（自己/他人）
{
    // 创建用户名标签
    m_pNameLabel = new QLabel();
    m_pNameLabel->setObjectName("chat_user_name");
    QFont font("Microsoft YaHei"); // 设置字体
    font.setPointSize(9);
    m_pNameLabel->setFont(font);
    m_pNameLabel->setFixedHeight(20); // 设置固定高度20像素

    // 创建头像标签
    m_pIconLabel = new QLabel();
    m_pIconLabel->setScaledContents(true); // 让头像自动适应标签大小
    m_pIconLabel->setFixedSize(42, 42);     // 头像固定为42x42像素

    // 创建气泡框（用于放置聊天内容）
    m_pBubble = new QWidget();

    // 创建网格布局器
    QGridLayout* pGLayout = new QGridLayout();
    pGLayout->setVerticalSpacing(3);     // 垂直方向控件间距3
    pGLayout->setHorizontalSpacing(3);   // 水平方向控件间距3
    pGLayout->setContentsMargins(3, 3, 3, 3); // 设置四周内边距为3

    // 创建一个弹性占位器（用于布局时推开控件）
    QSpacerItem* pSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    // 根据聊天角色不同，布局方向不同
    if (m_role == ChatRole::Self) // 自己发送的消息
    {
        m_pNameLabel->setContentsMargins(0, 0, 8, 0); // 右侧留8像素
        m_pNameLabel->setAlignment(Qt::AlignRight);   // 名字右对齐

        // 添加控件到网格布局（位置：行, 列, 占几行, 占几列）
        pGLayout->addWidget(m_pNameLabel, 0, 1, 1, 1);
        pGLayout->addWidget(m_pIconLabel, 0, 2, 2, 1, Qt::AlignTop); // 头像顶部对齐，占两行
        pGLayout->addItem(pSpacer, 1, 0, 1, 1);      // 左边添加弹性空白
        pGLayout->addWidget(m_pBubble, 1, 1, 1, 1);   // 气泡放在中间

        pGLayout->setColumnStretch(0, 2); // 第一列扩展比例为2
        pGLayout->setColumnStretch(1, 3); // 第二列扩展比例为3
    }
    else // 对方发送的消息
    {
        m_pNameLabel->setContentsMargins(8, 0, 0, 0); // 左侧留8像素
        m_pNameLabel->setAlignment(Qt::AlignLeft);    // 名字左对齐

        pGLayout->addWidget(m_pIconLabel, 0, 0, 2, 1, Qt::AlignTop); // 头像放最左侧
        pGLayout->addWidget(m_pNameLabel, 0, 1, 1, 1);               // 名字在头像右边
        pGLayout->addWidget(m_pBubble, 1, 1, 1, 1);                  // 气泡在名字下方
        pGLayout->addItem(pSpacer, 2, 2, 1, 1);                      // 最右边添加空白

        pGLayout->setColumnStretch(1, 3); // 中间列扩展比例为3
        pGLayout->setColumnStretch(2, 2); // 右边列扩展比例为2
    }

    // 将布局设置到当前控件
    this->setLayout(pGLayout);
}

// 设置用户名
void ChatItemBase::setUserName(const QString& name)
{
    m_pNameLabel->setText(name);
}

// 设置用户头像
void ChatItemBase::setUserIcon(const QPixmap& icon)
{
    m_pIconLabel->setPixmap(icon);
}

// 替换气泡控件中的内容
void ChatItemBase::setWidget(QWidget* w)
{
    // 获取当前布局
    QGridLayout* pGLayout = (qobject_cast<QGridLayout*>(this->layout()));

    // 用新的控件替换旧的气泡控件
    pGLayout->replaceWidget(m_pBubble, w);

    // 删除旧气泡控件
    delete m_pBubble;

    // 保存新控件
    m_pBubble = w;
}
