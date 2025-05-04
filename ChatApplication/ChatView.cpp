#include "ChatView.h"
#include <QScrollBar>
#include <QVBoxLayout>
#include <QEvent>
#include <QDebug>
#include <QTimer>
#include <QStyleOption>
#include <QPainter>

// ChatView 构造函数，初始化整个聊天窗口组件
ChatView::ChatView(QWidget* parent)
    : QWidget(parent)
    , isAppended(false) // 用于标记是否刚刚添加了新消息
{
    // 设置主布局为垂直布局，并去除边距
    QVBoxLayout* pMainLayout = new QVBoxLayout();
    this->setLayout(pMainLayout);
    pMainLayout->setContentsMargins(0, 0, 0, 0);

    // 创建滚动区域用于显示聊天内容
    m_pScrollArea = new QScrollArea();
    m_pScrollArea->setObjectName("chat_area");
    pMainLayout->addWidget(m_pScrollArea);

    // 创建一个容器用来放置所有聊天消息的控件
    QWidget* w = new QWidget(this);
    w->setObjectName("chat_bg");
    w->setAutoFillBackground(true); // 填充背景色（可由样式表控制）

    // 设置聊天消息布局（从上往下添加）
    QVBoxLayout* pHLayout_1 = new QVBoxLayout();
    pHLayout_1->addWidget(new QWidget(), 100000); // 占位控件，使消息靠上
    w->setLayout(pHLayout_1);

    // 将上面创建的容器设置为 QScrollArea 的子控件
    m_pScrollArea->setWidget(w); // 必须在 QScrollArea 创建后设置其 widget

    // 默认隐藏垂直滚动条
    m_pScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 获取滚动条并连接 rangeChanged 信号
    QScrollBar* pVScrollBar = m_pScrollArea->verticalScrollBar();
    connect(pVScrollBar, &QScrollBar::rangeChanged, this, &ChatView::onVScrollBarMoved);

    // 用一个水平布局将垂直滚动条放到右侧边缘
    QHBoxLayout* pHLayout_2 = new QHBoxLayout();
    pHLayout_2->addWidget(pVScrollBar, 0, Qt::AlignRight);
    pHLayout_2->setContentsMargins(0, 0, 0, 0);
    m_pScrollArea->setLayout(pHLayout_2); // 这一步把 ScrollArea 本身的布局设置为显示滚动条布局
    pVScrollBar->setHidden(true); // 初始时隐藏

    // 允许 scroll area 自动调整其子控件大小
    m_pScrollArea->setWidgetResizable(true);

    // 安装事件过滤器，用于监听鼠标进入离开事件
    m_pScrollArea->installEventFilter(this);

    // 初始化样式
    initStyleSheet();
}

// 添加聊天项到聊天窗口的底部
void ChatView::appendChatItem(QWidget* item)
{
    QVBoxLayout* vl = qobject_cast<QVBoxLayout*>(m_pScrollArea->widget()->layout());
    qDebug() << "vl->count() is " << vl->count();
    vl->insertWidget(vl->count() - 1, item); // 插入到倒数第二个位置，最后一个是占位控件
    isAppended = true; // 设置标志位，准备滚动到底部
}

// 预留：向顶部插入聊天项
void ChatView::prependChatItem(QWidget* item)
{
    // 待实现
}

// 预留：插入聊天项到指定控件之前
void ChatView::insertChatItem(QWidget* before, QWidget* item)
{
    // 待实现
}

// 删除所有聊天项（保留最后一个占位控件）
void ChatView::removeAllItem()
{
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(m_pScrollArea->widget()->layout());

    int count = layout->count();

    for (int i = 0; i < count - 1; ++i) { // 保留最后一个占位控件
        QLayoutItem* item = layout->takeAt(0); // 总是删除第一个控件
        if (item) {
            if (QWidget* widget = item->widget()) {
                delete widget; // 删除控件
            }
            delete item;
        }
    }
}

// 事件过滤器，用于控制滚动条的显隐
bool ChatView::eventFilter(QObject* o, QEvent* e)
{
    // 鼠标进入 scrollArea 区域时，显示滚动条（如果有内容）
    if (e->type() == QEvent::Enter && o == m_pScrollArea)
    {
        m_pScrollArea->verticalScrollBar()->setHidden(m_pScrollArea->verticalScrollBar()->maximum() == 0);
    }
    // 鼠标离开 scrollArea 区域时，隐藏滚动条
    else if (e->type() == QEvent::Leave && o == m_pScrollArea)
    {
        m_pScrollArea->verticalScrollBar()->setHidden(true);
    }
    return QWidget::eventFilter(o, e);
}

// 重绘事件，用于支持样式表中的背景绘制
void ChatView::paintEvent(QPaintEvent* event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this); // 支持样式表中的背景色、边框等
}

// 滚动条内容发生变化时自动滚动到底部（在添加新消息后）
void ChatView::onVScrollBarMoved(int min, int max)
{
    if (isAppended) // 如果刚刚添加了 item，则滚动到底部
    {
        QScrollBar* pVScrollBar = m_pScrollArea->verticalScrollBar();
        pVScrollBar->setSliderPosition(pVScrollBar->maximum()); // 滚动到底部

        // 启动一个延迟清除 isAppended 标志的定时器，避免多次滚动
        QTimer::singleShot(500, [this]()
            {
                isAppended = false;
            });
    }
}

// 设置滚动条样式（目前注释掉了）
void ChatView::initStyleSheet()
{
    // 以下样式设置滚动条为透明背景、红色滑块、无上下箭头，可根据需要取消注释启用
//    QScrollBar *scrollBar = m_pScrollArea->verticalScrollBar();
//    scrollBar->setStyleSheet("QScrollBar{background:transparent;}"
//                             "QScrollBar:vertical{background:transparent;width:8px;}"
//                             "QScrollBar::handle:vertical{background:red; border-radius:4px;min-height:20px;}"
//                             "QScrollBar::add-line:vertical{height:0px}"
//                             "QScrollBar::sub-line:vertical{height:0px}"
//                             "QScrollBar::add-page:vertical {background:transparent;}"
//                             "QScrollBar::sub-page:vertical {background:transparent;}");
}
