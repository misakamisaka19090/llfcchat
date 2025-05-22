#include "contactuserlist.h"
#include "global.h"
#include "listitembase.h"
#include "grouptipitem.h"
#include "conuseritem.h"
#include <QRandomGenerator>
#include "tcpmgr.h"
#include "usermgr.h"
#include <QTimer>
#include <QCoreApplication>
#include "usermgr.h"

ContactUserList::ContactUserList(QWidget *parent): _add_friend_item(nullptr)
  ,_load_pending(false)
{
    Q_UNUSED(parent);
     this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
     this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // 安装事件过滤器
     this->viewport()->installEventFilter(this);

    //模拟从数据库或者后端传输过来的数据,进行列表加载
    addContactUserList();
    //连接点击的信号和槽
    connect(this, &QListWidget::itemClicked, this, &ContactUserList::slot_item_clicked);
    //链接对端同意认证后通知的信号
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_add_auth_friend,this,
            &ContactUserList::slot_add_auth_firend);

    //链接自己点击同意认证后界面刷新
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_auth_rsp,this,
            &ContactUserList::slot_auth_rsp);
}


void ContactUserList::ShowRedPoint(bool bshow /*= true*/)
{
    _add_friend_item->ShowRedPoint(bshow);
}

void ContactUserList::addContactUserList()
{
    // 获取好友列表并初始化联系人列表界面

    // 添加“新的朋友”前的分组提示项
    auto* groupTip = new GroupTipItem();  // 自定义的分组提示控件（显示为一行文字）
    QListWidgetItem* item = new QListWidgetItem; // QListWidget的普通条目
    item->setSizeHint(groupTip->sizeHint()); // 设置条目高度与 groupTip 控件一致
    this->addItem(item);                     // 将条目添加到 QListWidget 中
    this->setItemWidget(item, groupTip);     // 设置 item 的显示控件为 groupTip（替换默认显示）
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable); // 设置该条目不可选中（只做展示用）

    // 添加“新的朋友”按钮项（用户可点击）
    _add_friend_item = new ConUserItem(); // 自定义联系人项（头像 + 名称）
    _add_friend_item->setObjectName("new_friend_item");
    _add_friend_item->SetInfo(0, tr("新的朋友"), ":/res/add_friend.png"); // 设置显示信息
    _add_friend_item->SetItemType(ListItemType::APPLY_FRIEND_ITEM); // 设置项类型（如申请类）

    //创建原生的QListWidgetItem
    QListWidgetItem* add_item = new QListWidgetItem;
    add_item->setSizeHint(_add_friend_item->sizeHint()); // 设置高度
    this->addItem(add_item);
    this->setItemWidget(add_item, _add_friend_item); // 替换 item 的显示控件为自定义的 ConUserItem
    this->setCurrentItem(add_item); // 默认选中此项

    // 添加“联系人”分组提示
    auto* groupCon = new GroupTipItem();
    groupCon->SetGroupTip(tr("联系人")); // 设置提示文字
    _groupitem = new QListWidgetItem;
    _groupitem->setSizeHint(groupCon->sizeHint());
    this->addItem(_groupitem);
    this->setItemWidget(_groupitem, groupCon);
    _groupitem->setFlags(_groupitem->flags() & ~Qt::ItemIsSelectable); // 不可选中

    // 加载后端提供的联系人列表
    auto con_list = UserMgr::GetInstance()->GetConListPerPage();
    for (auto& con_ele : con_list) {
        auto* con_user_wid = new ConUserItem();
        con_user_wid->SetInfo(con_ele->_uid, con_ele->_name, con_ele->_icon); // 设置联系人信息
        QListWidgetItem* item = new QListWidgetItem;
        item->setSizeHint(con_user_wid->sizeHint());
        this->addItem(item);
        this->setItemWidget(item, con_user_wid); // 设置自定义控件显示内容
    }

    UserMgr::GetInstance()->UpdateContactLoadedCount(); // 通知已加载完一页联系人

    // 下面是模拟数据，用于测试/演示界面效果
    for (int i = 0; i < 13; i++) {
        int randomValue = QRandomGenerator::global()->bounded(100); // 生成 0~99 的随机数
        int str_i = randomValue % strs.size();
        int head_i = randomValue % heads.size();
        int name_i = randomValue % names.size();

        auto* con_user_wid = new ConUserItem();
        con_user_wid->SetInfo(0, names[name_i], heads[head_i]); // 设置模拟联系人数据
        QListWidgetItem* item = new QListWidgetItem;
        item->setSizeHint(con_user_wid->sizeHint());
        this->addItem(item);
        this->setItemWidget(item, con_user_wid); // 设置自定义控件显示内容
    }
}

bool ContactUserList::eventFilter(QObject *watched, QEvent *event)
{
    // 检查事件是否是鼠标悬浮进入或离开
    if (watched == this->viewport()) {
        if (event->type() == QEvent::Enter) {
            // 鼠标悬浮，显示滚动条
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        } else if (event->type() == QEvent::Leave) {
            // 鼠标离开，隐藏滚动条
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        }
    }

    // 检查事件是否是鼠标滚轮事件
    if (watched == this->viewport() && event->type() == QEvent::Wheel) {
        QWheelEvent *wheelEvent = static_cast<QWheelEvent*>(event);
        int numDegrees = wheelEvent->angleDelta().y() / 8;
        int numSteps = numDegrees / 15; // 计算滚动步数

        // 设置滚动幅度
        this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() - numSteps);

        // 检查是否滚动到底部
        QScrollBar *scrollBar = this->verticalScrollBar();
        int maxScrollValue = scrollBar->maximum();
        int currentValue = scrollBar->value();
        //int pageSize = 10; // 每页加载的联系人数量

        if (maxScrollValue - currentValue <= 0) {

            auto b_loaded = UserMgr::GetInstance()->IsLoadChatFin();
            if(b_loaded){
                return true;
            }

            if(_load_pending){
                return true;
            }

            _load_pending = true;

            QTimer::singleShot(100, [this](){
                _load_pending = false;
                QCoreApplication::quit(); // 完成后退出应用程序
                });
            // 滚动到底部，加载新的联系人
            qDebug()<<"load more contact user";
            //发送信号通知聊天界面加载更多聊天内容
            emit sig_loading_contact_user();
         }

        return true; // 停止事件传递
    }

    return QListWidget::eventFilter(watched, event);

}

void ContactUserList::slot_item_clicked(QListWidgetItem *item)
{
    QWidget *widget = this->itemWidget(item); // 获取自定义widget对象
    if(!widget){
        qDebug()<< "slot item clicked widget is nullptr";
        return;
    }

    // 对自定义widget进行操作， 将item 转化为基类ListItemBase
    ListItemBase *customItem = qobject_cast<ListItemBase*>(widget);
    if(!customItem){
        qDebug()<< "slot item clicked widget is nullptr";
        return;
    }

    auto itemType = customItem->GetItemType();
    if(itemType == ListItemType::INVALID_ITEM
            || itemType == ListItemType::GROUP_TIP_ITEM){
        qDebug()<< "slot invalid item clicked ";
        return;
    }

   if(itemType == ListItemType::APPLY_FRIEND_ITEM){

       // 创建对话框，提示用户
       qDebug()<< "apply friend item clicked ";
       //跳转到好友申请界面
       emit sig_switch_apply_friend_page(); 
       return;
   }

   if(itemType == ListItemType::CONTACT_USER_ITEM){
       // 创建对话框，提示用户
       qDebug()<< "contact user item clicked ";

       auto con_item = qobject_cast<ConUserItem*>(customItem);
       auto user_info = con_item->GetInfo();
       //跳转到好友申请界面
       emit sig_switch_friend_info_page(user_info);
       return;
   }
}

void ContactUserList::slot_add_auth_firend(std::shared_ptr<AuthInfo> auth_info)
{
    qDebug() << "slot add auth friend ";
    bool isFriend = UserMgr::GetInstance()->CheckFriendById(auth_info->_uid);
    if(isFriend){
        return;
    }
    // 在 groupitem 之后插入新项
    int randomValue = QRandomGenerator::global()->bounded(100); // 生成0到99之间的随机整数
    int str_i = randomValue%strs.size();
    int head_i = randomValue%heads.size();

    auto *con_user_wid = new ConUserItem();
    con_user_wid->SetInfo(auth_info);
    QListWidgetItem *item = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item->setSizeHint(con_user_wid->sizeHint());

    // 获取 groupitem 的索引
    int index = this->row(_groupitem);
    // 在 groupitem 之后插入新项
    this->insertItem(index + 1, item);

    this->setItemWidget(item, con_user_wid);

}

void ContactUserList::slot_auth_rsp(std::shared_ptr<AuthRsp> auth_rsp)
{
    qDebug() << "slot auth rsp called";
    bool isFriend = UserMgr::GetInstance()->CheckFriendById(auth_rsp->_uid);
    if(isFriend){
        return;
    }
    // 在 groupitem 之后插入新项
    int randomValue = QRandomGenerator::global()->bounded(100); // 生成0到99之间的随机整数
    int str_i = randomValue%strs.size();
    int head_i = randomValue%heads.size();

    auto *con_user_wid = new ConUserItem();
    con_user_wid->SetInfo(auth_rsp->_uid ,auth_rsp->_name, heads[head_i]);
    QListWidgetItem *item = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item->setSizeHint(con_user_wid->sizeHint());

    // 获取 groupitem 的索引
    int index = this->row(_groupitem);
    // 在 groupitem 之后插入新项
    this->insertItem(index + 1, item);

    this->setItemWidget(item, con_user_wid);

}


