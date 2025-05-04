#include "chatdialog.h"
#include "ui_chatdialog.h"
#include <QAction>
#include "chatuserwid.h"
#include <QDebug>
#include <vector>
#include <QRandomGenerator>
#include "loadingdlg.h"
#include "global.h"
#include "ChatItemBase.h"
#include "TextBubble.h"
#include "PictureBubble.h"
#include "MessageTextEdit.h"
#include "chatuserlist.h"
#include "grouptipitem.h"
#include "invaliditem.h"
#include "conuseritem.h"
#include "lineitem.h"
#include "tcpmgr.h"
#include "usermgr.h"


// 构造函数：初始化聊天对话框，设置界面控件及信号槽连接
ChatDialog::ChatDialog(QWidget *parent) :
    QDialog(parent), // 调用父类QDialog的构造函数
    ui(new Ui::ChatDialog), // 初始化UI界面
    _b_loading(false), // 初始化加载标志
    _mode(ChatUIMode::ChatMode), // 设置默认UI模式为聊天模式
    _state(ChatUIMode::ChatMode), // 设置当前状态为聊天模式
    _last_widget(nullptr), // 初始化最后一个小部件指针为空
    _cur_chat_uid(0) // 初始化当前聊天用户ID为0
{
    ui->setupUi(this); // 设置UI界面，绑定UI控件

    // 设置“添加”按钮的状态为正常、悬停、按下状态
    ui->add_btn->SetState("normal", "hover", "press");
    ui->add_btn->setProperty("state", "normal"); // 设置按钮的初始状态

    // 创建一个搜索框的动作，并为该动作设置图标
    QAction *searchAction = new QAction(ui->search_edit);
    searchAction->setIcon(QIcon(":/res/search.png")); // 设置搜索图标
    ui->search_edit->addAction(searchAction, QLineEdit::LeadingPosition); // 将搜索图标添加到搜索框的左侧
    ui->search_edit->setPlaceholderText(QStringLiteral("搜索")); // 设置搜索框的提示文本

    // 创建一个清除动作并设置图标
    QAction* clearAction = new QAction(ui->search_edit);
    clearAction->setIcon(QIcon(":/res/close_transparent.png"));
    // 初始时不显示清除图标
    // 将清除动作添加到LineEdit的末尾位置
    ui->search_edit->addAction(clearAction, QLineEdit::TrailingPosition);

    // 当文本框内容变化时，更新清除图标的状态
    connect(ui->search_edit, &QLineEdit::textChanged, [clearAction](const QString &text) {
        if (!text.isEmpty()) {
            clearAction->setIcon(QIcon(":/res/close_search.png")); // 文本非空时显示清除图标
        } else {
            clearAction->setIcon(QIcon(":/res/close_transparent.png")); // 文本为空时显示透明图标
        }
    });

    // 清除文本框内容并隐藏清除图标
    connect(clearAction, &QAction::triggered, [this, clearAction]() {
        ui->search_edit->clear(); // 清空搜索框内容
        clearAction->setIcon(QIcon(":/res/close_transparent.png")); // 清空内容后将清除图标变为透明
        ui->search_edit->clearFocus(); // 清除焦点
        ShowSearch(false); // 隐藏搜索框
    });

    ui->search_edit->SetMaxLength(15); // 限制搜索框输入的最大长度为15个字符

    // 连接加载聊天用户的信号和槽
    connect(ui->chat_user_list, &ChatUserList::sig_loading_chat_user, this, &ChatDialog::slot_loading_chat_user);
    addChatUserList(); // 添加聊天用户列表

    // 模拟加载自己的头像并设置为头像显示框
    QString head_icon = UserMgr::GetInstance()->GetIcon(); // 获取用户头像路径
    QPixmap pixmap(head_icon); // 加载头像图片
    QPixmap scaledPixmap = pixmap.scaled(ui->side_head_lb->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation); // 缩放头像
    ui->side_head_lb->setPixmap(scaledPixmap); // 设置缩放后的头像到QLabel
    ui->side_head_lb->setScaledContents(true); // 设置QLabel自动缩放图片内容以适应大小

    ui->side_chat_lb->setProperty("state", "normal"); // 设置聊天页面状态为正常
    ui->side_chat_lb->SetState("normal", "hover", "pressed", "selected_normal", "selected_hover", "selected_pressed"); // 设置聊天页面的不同状态
    ui->side_contact_lb->SetState("normal", "hover", "pressed", "selected_normal", "selected_hover", "selected_pressed"); // 设置联系人页面的不同状态

    // 将聊天页面和联系人页面添加到控件组中
    AddLBGroup(ui->side_chat_lb);
    AddLBGroup(ui->side_contact_lb);

    // 连接侧边栏聊天按钮的点击信号到槽函数
    connect(ui->side_chat_lb, &StateWidget::clicked, this, &ChatDialog::slot_side_chat);
    connect(ui->side_contact_lb, &StateWidget::clicked, this, &ChatDialog::slot_side_contact);

    // 连接搜索框文本变化的信号到槽函数
    connect(ui->search_edit, &QLineEdit::textChanged, this, &ChatDialog::slot_text_changed);

    ShowSearch(false); // 默认不显示搜索框

    // 安装事件过滤器，用于处理鼠标点击事件判断是否要清空搜索框
    this->installEventFilter(this);

    ui->side_chat_lb->SetSelected(true); // 默认选择聊天页面
    SetSelectChatItem(); // 设置选中的聊天项
    SetSelectChatPage(); // 设置聊天界面信息

    // 连接加载联系人列表的信号和槽
    connect(ui->con_user_list, &ContactUserList::sig_loading_contact_user, this, &ChatDialog::slot_loading_contact_user);

    // 连接联系人页面点击好友申请条目的信号
    connect(ui->con_user_list, &ContactUserList::sig_switch_apply_friend_page, this, &ChatDialog::slot_switch_apply_friend_page);

    // 连接清除搜索框操作的信号
    connect(ui->friend_apply_page, &ApplyFriendPage::sig_show_search, this, &ChatDialog::slot_show_search);

    // 为搜索列表设置搜索框
    ui->search_list->SetSearchEdit(ui->search_edit);

    // 连接申请添加好友信号
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_friend_apply, this, &ChatDialog::slot_apply_friend);

    // 连接认证添加好友信号
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_add_auth_friend, this, &ChatDialog::slot_add_auth_friend);

    // 连接自己认证回复信号
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_auth_rsp, this, &ChatDialog::slot_auth_rsp);

    // 连接点击联系人项显示用户信息的信号
    connect(ui->con_user_list, &ContactUserList::sig_switch_friend_info_page, this, &ChatDialog::slot_friend_info_page);

    // 设置中心部件为聊天页面
    ui->stackedWidget->setCurrentWidget(ui->chat_page);

    // 连接搜索列表跳转聊天信号
    connect(ui->search_list, &SearchList::sig_jump_chat_item, this, &ChatDialog::slot_jump_chat_item);

    // 连接好友信息界面点击事件跳转聊天
    connect(ui->friend_info_page, &FriendInfoPage::sig_jump_chat_item, this, &ChatDialog::slot_jump_chat_item_from_infopage);

    // 连接聊天用户列表项点击事件
    connect(ui->chat_user_list, &QListWidget::itemClicked, this, &ChatDialog::slot_item_clicked);

    // 连接对端消息通知信号
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_text_chat_msg, this, &ChatDialog::slot_text_chat_msg);

    // 连接发送聊天消息的槽函数
    connect(ui->chat_page, &ChatPage::sig_append_send_chat_msg, this, &ChatDialog::slot_append_send_chat_msg);
}


ChatDialog::~ChatDialog()
{
    delete ui;
}

void ChatDialog::slot_item_clicked(QListWidgetItem *item)
{
    QWidget *widget = ui->chat_user_list->itemWidget(item); // 获取自定义widget对象
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


   if(itemType == ListItemType::CHAT_USER_ITEM){
       // 创建对话框，提示用户
       qDebug()<< "contact user item clicked ";

       auto chat_wid = qobject_cast<ChatUserWid*>(customItem);
       auto user_info = chat_wid->GetUserInfo();
       //跳转到聊天界面
       ui->chat_page->SetUserInfo(user_info);
       _cur_chat_uid = user_info->_uid;
       return;
   }
}

void ChatDialog::slot_text_chat_msg(std::shared_ptr<TextChatMsg> msg)
{
    auto find_iter = _chat_items_added.find(msg->_from_uid);
    if(find_iter != _chat_items_added.end()){
        qDebug() << "set chat item msg, uid is " << msg->_from_uid;
        QWidget *widget = ui->chat_user_list->itemWidget(find_iter.value());
        auto chat_wid = qobject_cast<ChatUserWid*>(widget);
        if(!chat_wid){
            return;
        }
        chat_wid->updateLastMsg(msg->_chat_msgs);
        //更新当前聊天页面记录
        UpdateChatMsg(msg->_chat_msgs);
        UserMgr::GetInstance()->AppendFriendChatMsg(msg->_from_uid,msg->_chat_msgs);
        return;
    }

    //如果没找到，则创建新的插入listwidget

    auto* chat_user_wid = new ChatUserWid();
    //查询好友信息
    auto fi_ptr = UserMgr::GetInstance()->GetFriendById(msg->_from_uid);
    chat_user_wid->SetInfo(fi_ptr);
    QListWidgetItem* item = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item->setSizeHint(chat_user_wid->sizeHint());
    chat_user_wid->updateLastMsg(msg->_chat_msgs);
     UserMgr::GetInstance()->AppendFriendChatMsg(msg->_from_uid,msg->_chat_msgs);
    ui->chat_user_list->insertItem(0, item);
    ui->chat_user_list->setItemWidget(item, chat_user_wid);
    _chat_items_added.insert(msg->_from_uid, item);

}


// 事件过滤器，用于捕捉鼠标按下事件
bool ChatDialog::eventFilter(QObject* watched, QEvent* event)
{
    // 判断事件类型是否为鼠标按下事件
    if (event->type() == QEvent::MouseButtonPress) {
        // 将 QEvent 类型转换为 QMouseEvent 类型，以便访问鼠标事件的具体信息
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        // 处理全局鼠标按下事件
        handleGlobalMousePress(mouseEvent);
    }
    // 调用基类的 eventFilter 方法，继续事件的处理
    return QDialog::eventFilter(watched, event);
}

// 处理全局鼠标按下事件
void ChatDialog::handleGlobalMousePress(QMouseEvent* event)
{
    // 如果当前不处于搜索模式，直接返回，不做任何处理
    if (_mode != ChatUIMode::SearchMode) {
        return;
    }

    // 将鼠标点击位置从全局坐标系转换为搜索列表的局部坐标系
    QPoint posInSearchList = ui->search_list->mapFromGlobal(event->globalPos());

    // 判断鼠标点击位置是否在搜索列表的有效区域内
    if (!ui->search_list->rect().contains(posInSearchList)) {
        // 如果点击位置不在聊天列表内，清空搜索输入框的内容
        ui->search_edit->clear();
        // 隐藏搜索模式的UI组件
        ShowSearch(false);
    }
}


void ChatDialog::CloseFindDlg()
{
    ui->search_list->CloseFindDlg();
}

void ChatDialog::UpdateChatMsg(std::vector<std::shared_ptr<TextChatData> > msgdata)
{
    for(auto & msg : msgdata){
        if(msg->_from_uid != _cur_chat_uid){
            break;
        }

        ui->chat_page->AppendChatMsg(msg);
    }
}

void ChatDialog::slot_append_send_chat_msg(std::shared_ptr<TextChatData> msgdata) {
    if (_cur_chat_uid == 0) {
        return;
    }

    auto find_iter = _chat_items_added.find(_cur_chat_uid);
    if (find_iter == _chat_items_added.end()) {
        return;
    }

    //转为widget
    QWidget* widget = ui->chat_user_list->itemWidget(find_iter.value());
    if (!widget) {
        return;
    }

    //判断转化为自定义的widget
    // 对自定义widget进行操作， 将item 转化为基类ListItemBase
    ListItemBase* customItem = qobject_cast<ListItemBase*>(widget);
    if (!customItem) {
        qDebug() << "qobject_cast<ListItemBase*>(widget) is nullptr";
        return;
    }

    auto itemType = customItem->GetItemType();
    if (itemType == CHAT_USER_ITEM) {
        auto con_item = qobject_cast<ChatUserWid*>(customItem);
        if (!con_item) {
            return;
        }

        //设置信息
        auto user_info = con_item->GetUserInfo();
        user_info->_chat_msgs.push_back(msgdata);
        std::vector<std::shared_ptr<TextChatData>> msg_vec;
        msg_vec.push_back(msgdata);
        UserMgr::GetInstance()->AppendFriendChatMsg(_cur_chat_uid,msg_vec);
        return;
    }
}

void ChatDialog::AddLBGroup(StateWidget* lb)
{
    _lb_list.push_back(lb);
}



void ChatDialog::addChatUserList()
{
    //先按照好友列表加载聊天记录，等以后客户端实现聊天记录数据库之后再按照最后信息排序
    auto friend_list = UserMgr::GetInstance()->GetChatListPerPage();
    if (friend_list.empty() == false) {
        for(auto & friend_ele : friend_list){
            auto find_iter = _chat_items_added.find(friend_ele->_uid);
            if(find_iter != _chat_items_added.end()){
                continue;
            }
            auto *chat_user_wid = new ChatUserWid();
            auto user_info = std::make_shared<UserInfo>(friend_ele);
            chat_user_wid->SetInfo(user_info);
            QListWidgetItem *item = new QListWidgetItem;
            //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
            item->setSizeHint(chat_user_wid->sizeHint());
            ui->chat_user_list->addItem(item);
            ui->chat_user_list->setItemWidget(item, chat_user_wid);
            _chat_items_added.insert(friend_ele->_uid, item);
        }

        //更新已加载条目
        UserMgr::GetInstance()->UpdateChatLoadedCount();
    }

    //模拟测试条目
    // 创建QListWidgetItem，并设置自定义的widget
    for(int i = 0; i < 13; i++){
        int randomValue = QRandomGenerator::global()->bounded(100); // 生成0到99之间的随机整数
        int str_i = randomValue%strs.size();
        int head_i = randomValue%heads.size();
        int name_i = randomValue%names.size();

        auto *chat_user_wid = new ChatUserWid();
        auto user_info = std::make_shared<UserInfo>(0,names[name_i],
                                                    names[name_i],heads[head_i],0,strs[str_i]);
        chat_user_wid->SetInfo(user_info);
        QListWidgetItem *item = new QListWidgetItem;
        //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
        item->setSizeHint(chat_user_wid->sizeHint());
        ui->chat_user_list->addItem(item);
        ui->chat_user_list->setItemWidget(item, chat_user_wid);
    }

}

void ChatDialog::loadMoreChatUser() {
    // 获取每一页的聊天用户列表
    auto friend_list = UserMgr::GetInstance()->GetChatListPerPage();

    // 如果获取的列表不为空，执行以下操作
    if (friend_list.empty() == false) {
        // 遍历每个聊天用户
        for (auto &friend_ele : friend_list) {
            // 检查该聊天用户是否已经添加过，如果已经添加过则跳过
            auto find_iter = _chat_items_added.find(friend_ele->_uid);
            if (find_iter != _chat_items_added.end()) {
                continue;  // 如果已存在，跳过当前用户
            }

            // 创建一个新的聊天用户窗口小部件
            auto *chat_user_wid = new ChatUserWid();

            // 将聊天用户的信息传递给聊天用户窗口小部件
            auto user_info = std::make_shared<UserInfo>(friend_ele);
            chat_user_wid->SetInfo(user_info);

            // 创建一个新的列表项（QListWidgetItem），用于显示聊天用户
            QListWidgetItem *item = new QListWidgetItem;

            // 设置列表项的大小，使其与聊天用户窗口小部件的大小相匹配
            item->setSizeHint(chat_user_wid->sizeHint());

            // 将列表项添加到聊天用户列表中
            ui->chat_user_list->addItem(item);

            // 将聊天用户窗口小部件添加到对应的列表项中
            ui->chat_user_list->setItemWidget(item, chat_user_wid);

            // 将用户的UID和对应的列表项添加到已加载项的集合中，避免重复添加
            _chat_items_added.insert(friend_ele->_uid, item);
        }

        // 更新已加载的聊天用户条目数
        UserMgr::GetInstance()->UpdateChatLoadedCount();
    }
}


void ChatDialog::ClearLabelState(StateWidget *lb)
{
    for(auto & ele: _lb_list){
        if(ele == lb){
            continue;
        }

        ele->ClearState();
    }
}

void ChatDialog::loadMoreConUser()
{
    auto friend_list = UserMgr::GetInstance()->GetConListPerPage();
    if (friend_list.empty() == false) {
        for(auto & friend_ele : friend_list){
            auto *chat_user_wid = new ConUserItem();
            chat_user_wid->SetInfo(friend_ele->_uid,friend_ele->_name,
                                   friend_ele->_icon);
            QListWidgetItem *item = new QListWidgetItem;
            //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
            item->setSizeHint(chat_user_wid->sizeHint());
            ui->con_user_list->addItem(item);
            ui->con_user_list->setItemWidget(item, chat_user_wid);
        }

        //更新已加载条目
        UserMgr::GetInstance()->UpdateContactLoadedCount();
    }
}

void ChatDialog::SetSelectChatItem(int uid)
{
    if(ui->chat_user_list->count() <= 0){
        return;
    }

    if(uid == 0){
        ui->chat_user_list->setCurrentRow(0);
        QListWidgetItem *firstItem = ui->chat_user_list->item(0);
        if(!firstItem){
            return;
        }

        //转为widget
        QWidget *widget = ui->chat_user_list->itemWidget(firstItem);
        if(!widget){
            return;
        }

        auto con_item = qobject_cast<ChatUserWid*>(widget);
        if(!con_item){
            return;
        }

        _cur_chat_uid = con_item->GetUserInfo()->_uid;

        return;
    }

    auto find_iter = _chat_items_added.find(uid);
    if(find_iter == _chat_items_added.end()){
        qDebug() << "uid " <<uid<< " not found, set curent row 0";
        ui->chat_user_list->setCurrentRow(0);
        return;
    }

    ui->chat_user_list->setCurrentItem(find_iter.value());

    _cur_chat_uid = uid;
}

void ChatDialog::SetSelectChatPage(int uid)
{
    if( ui->chat_user_list->count() <= 0){
        return;
    }

    if (uid == 0) {
       auto item = ui->chat_user_list->item(0);
       //转为widget
       QWidget* widget = ui->chat_user_list->itemWidget(item);
       if (!widget) {
           return;
       }

       auto con_item = qobject_cast<ChatUserWid*>(widget);
       if (!con_item) {
           return;
       }

       //设置信息
       auto user_info = con_item->GetUserInfo();
       ui->chat_page->SetUserInfo(user_info);
       return;
    }

    auto find_iter = _chat_items_added.find(uid);
    if(find_iter == _chat_items_added.end()){
        return;
    }

    //转为widget
    QWidget *widget = ui->chat_user_list->itemWidget(find_iter.value());
    if(!widget){
        return;
    }

    //判断转化为自定义的widget
    // 对自定义widget进行操作， 将item 转化为基类ListItemBase
    ListItemBase *customItem = qobject_cast<ListItemBase*>(widget);
    if(!customItem){
        qDebug()<< "qobject_cast<ListItemBase*>(widget) is nullptr";
        return;
    }

    auto itemType = customItem->GetItemType();
    if(itemType == CHAT_USER_ITEM){
        auto con_item = qobject_cast<ChatUserWid*>(customItem);
        if(!con_item){
            return;
        }

        //设置信息
        auto user_info = con_item->GetUserInfo();
        ui->chat_page->SetUserInfo(user_info);

        return;
    }

}


void ChatDialog::ShowSearch(bool bsearch)
{
    if(bsearch){
        ui->chat_user_list->hide();
        ui->con_user_list->hide();
        ui->search_list->show();
        _mode = ChatUIMode::SearchMode;
    }else if(_state == ChatUIMode::ChatMode){
        ui->chat_user_list->show();
        ui->con_user_list->hide();
        ui->search_list->hide();
        _mode = ChatUIMode::ChatMode;
        ui->search_list->CloseFindDlg();
        ui->search_edit->clear();
        ui->search_edit->clearFocus();
    }
    else if (_state == ChatUIMode::ContactMode) {
        // 隐藏聊天用户列表（即主界面上的聊天会话列表）
        ui->chat_user_list->hide();
        // 隐藏搜索结果列表（搜索用户后展示的结果）
        ui->search_list->hide();
        // 显示联系人列表（比如好友申请、通讯录等）
        ui->con_user_list->show();
        // 设置当前模式为联系人模式
        _mode = ChatUIMode::ContactMode;
        // 关闭搜索结果中弹出的查找窗口（比如“未找到用户”或者“找到用户”的弹框）
        ui->search_list->CloseFindDlg();
        // 清空搜索输入框的内容
        ui->search_edit->clear();
        // 取消搜索输入框的焦点（即不再高亮或处于可输入状态）
        ui->search_edit->clearFocus();
    }
}

// 该槽函数用于加载聊天用户
void ChatDialog::slot_loading_chat_user()
{
    // 如果当前正在加载，则不执行任何操作，避免重复加载
    if(_b_loading){
        return;
    }

    // 标记正在加载状态
    _b_loading = true;

    // 创建一个加载对话框，并设置为模态对话框（即阻塞当前操作，直到对话框关闭）
    LoadingDlg *loadingDialog = new LoadingDlg(this);
    loadingDialog->setModal(true);
    loadingDialog->show(); // 显示加载对话框

    // 打印调试信息，表示正在向列表添加新数据
    qDebug() << "add new data to list.....";

    // 调用加载更多聊天用户的函数
    loadMoreChatUser();

    // 加载完成后，删除加载对话框
    loadingDialog->deleteLater();

    // 加载完成，设置为不再处于加载状态
    _b_loading = false;
}


void ChatDialog::slot_side_chat()
{
    qDebug()<< "receive side chat clicked";
    ClearLabelState(ui->side_chat_lb);
    ui->stackedWidget->setCurrentWidget(ui->chat_page);
    _state = ChatUIMode::ChatMode;
    ShowSearch(false);
}

void ChatDialog::slot_side_contact() {
    // 输出调试信息，表示接收到侧边栏“联系人”按钮的点击事件
    qDebug() << "receive side contact clicked";

    // 清除侧边栏联系人标签的状态
    ClearLabelState(ui->side_contact_lb);

    // 判断是否已经有上次显示的页面
    if (_last_widget == nullptr) {
        // 如果没有上次的页面，则将当前页面设置为“好友申请”页面
        ui->stackedWidget->setCurrentWidget(ui->friend_apply_page);
        _last_widget = ui->friend_apply_page; // 保存当前页面为上次页面
    }
    else {
        // 如果有上次的页面，则恢复显示上次的页面
        ui->stackedWidget->setCurrentWidget(_last_widget);
    }

    // 设置当前界面状态为“联系人模式”，即显示联系人相关功能
    _state = ChatUIMode::ContactMode;

    // 隐藏搜索框（表示切换到联系人模式时不需要显示搜索框）
    ShowSearch(false);
}


void ChatDialog::slot_text_changed(const QString &str)
{
    //qDebug()<< "receive slot text changed str is " << str;
    if (!str.isEmpty()) {
        ShowSearch(true);
    }
}

void ChatDialog::slot_focus_out()
{
    qDebug()<< "receive focus out signal";
    ShowSearch(false);
}

void ChatDialog::slot_loading_contact_user()
{
    qDebug() << "slot loading contact user";
    if(_b_loading){
        return;
    }

    _b_loading = true;
    LoadingDlg *loadingDialog = new LoadingDlg(this);
    loadingDialog->setModal(true);
    loadingDialog->show();
    qDebug() << "add new data to list.....";
    loadMoreConUser();
    // 加载完成后关闭对话框
    loadingDialog->deleteLater();

    _b_loading = false;
}

void ChatDialog::slot_switch_apply_friend_page()
{
    qDebug()<<"receive switch apply friend page sig";
    _last_widget = ui->friend_apply_page;
    ui->stackedWidget->setCurrentWidget(ui->friend_apply_page);
}

void ChatDialog::slot_friend_info_page(std::shared_ptr<UserInfo> user_info)
{
    qDebug()<<"receive switch friend info page sig";
    _last_widget = ui->friend_info_page;
    ui->stackedWidget->setCurrentWidget(ui->friend_info_page);
    ui->friend_info_page->SetInfo(user_info);
}



void ChatDialog::slot_show_search(bool show)
{
    ShowSearch(show);
}

void ChatDialog::slot_apply_friend(std::shared_ptr<AddFriendApply> apply)
{
    qDebug() << "receive apply friend slot, applyuid is " << apply->_from_uid << " name is "
        << apply->_name << " desc is " << apply->_desc;

   bool b_already = UserMgr::GetInstance()->AlreadyApply(apply->_from_uid);
   if(b_already){
        return;
   }

   UserMgr::GetInstance()->AddApplyList(std::make_shared<ApplyInfo>(apply));
    ui->side_contact_lb->ShowRedPoint(true);
    ui->con_user_list->ShowRedPoint(true);
    ui->friend_apply_page->AddNewApply(apply);
}

void ChatDialog::slot_add_auth_friend(std::shared_ptr<AuthInfo> auth_info) {
    qDebug() << "receive slot_add_auth__friend uid is " << auth_info->_uid
        << " name is " << auth_info->_name << " nick is " << auth_info->_nick;

    //判断如果已经是好友则跳过
    auto bfriend = UserMgr::GetInstance()->CheckFriendById(auth_info->_uid);
    if(bfriend){
        return;
    }

    UserMgr::GetInstance()->AddFriend(auth_info);

    int randomValue = QRandomGenerator::global()->bounded(100); // 生成0到99之间的随机整数
    int str_i = randomValue % strs.size();
    int head_i = randomValue % heads.size();
    int name_i = randomValue % names.size();

    auto* chat_user_wid = new ChatUserWid();
    auto user_info = std::make_shared<UserInfo>(auth_info);
    chat_user_wid->SetInfo(user_info);
    QListWidgetItem* item = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item->setSizeHint(chat_user_wid->sizeHint());
    ui->chat_user_list->insertItem(0, item);
    ui->chat_user_list->setItemWidget(item, chat_user_wid);
    _chat_items_added.insert(auth_info->_uid, item);
}

void ChatDialog::slot_auth_rsp(std::shared_ptr<AuthRsp> auth_rsp)
{
    qDebug() << "receive slot_auth_rsp uid is " << auth_rsp->_uid
        << " name is " << auth_rsp->_name << " nick is " << auth_rsp->_nick;

    //判断如果已经是好友则跳过
    auto bfriend = UserMgr::GetInstance()->CheckFriendById(auth_rsp->_uid);
    if(bfriend){
        return;
    }

    UserMgr::GetInstance()->AddFriend(auth_rsp);
    int randomValue = QRandomGenerator::global()->bounded(100); // 生成0到99之间的随机整数
    int str_i = randomValue % strs.size();
    int head_i = randomValue % heads.size();
    int name_i = randomValue % names.size();

    auto* chat_user_wid = new ChatUserWid();
    auto user_info = std::make_shared<UserInfo>(auth_rsp);
    chat_user_wid->SetInfo(user_info);
    QListWidgetItem* item = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item->setSizeHint(chat_user_wid->sizeHint());
    ui->chat_user_list->insertItem(0, item);
    ui->chat_user_list->setItemWidget(item, chat_user_wid);
    _chat_items_added.insert(auth_rsp->_uid, item);
}

void ChatDialog::slot_jump_chat_item(std::shared_ptr<SearchInfo> si)
{
    qDebug() << "slot jump chat item";
    auto find_iter = _chat_items_added.find(si->_uid);
    if(find_iter != _chat_items_added.end()){
        qDebug() << "jump to chat item , uid is " << si->_uid;
        ui->chat_user_list->scrollToItem(find_iter.value());
        ui->side_chat_lb->SetSelected(true);
        SetSelectChatItem(si->_uid);
        //更新聊天界面信息
        SetSelectChatPage(si->_uid);
        slot_side_chat();
        return;
    }

    //如果没找到，则创建新的插入listwidget

    auto* chat_user_wid = new ChatUserWid();
    auto user_info = std::make_shared<UserInfo>(si);
    chat_user_wid->SetInfo(user_info);
    QListWidgetItem* item = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item->setSizeHint(chat_user_wid->sizeHint());
    ui->chat_user_list->insertItem(0, item);
    ui->chat_user_list->setItemWidget(item, chat_user_wid);

    _chat_items_added.insert(si->_uid, item);

    ui->side_chat_lb->SetSelected(true);
    SetSelectChatItem(si->_uid);
    //更新聊天界面信息
    SetSelectChatPage(si->_uid);
    slot_side_chat();

}

void ChatDialog::slot_jump_chat_item_from_infopage(std::shared_ptr<UserInfo> user_info)
{
    qDebug() << "slot jump chat item ";
    auto find_iter = _chat_items_added.find(user_info->_uid);
    if(find_iter != _chat_items_added.end()){
        qDebug() << "jump to chat item , uid is " << user_info->_uid;
        ui->chat_user_list->scrollToItem(find_iter.value());
        ui->side_chat_lb->SetSelected(true);
        SetSelectChatItem(user_info->_uid);
        //更新聊天界面信息
        SetSelectChatPage(user_info->_uid);
        slot_side_chat();
        return;
    }

    //如果没找到，则创建新的插入listwidget

    auto* chat_user_wid = new ChatUserWid();
    chat_user_wid->SetInfo(user_info);
    QListWidgetItem* item = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item->setSizeHint(chat_user_wid->sizeHint());
    ui->chat_user_list->insertItem(0, item);
    ui->chat_user_list->setItemWidget(item, chat_user_wid);

    _chat_items_added.insert(user_info->_uid, item);

    ui->side_chat_lb->SetSelected(true);
    SetSelectChatItem(user_info->_uid);
    //更新聊天界面信息
    SetSelectChatPage(user_info->_uid);
    slot_side_chat();
}

