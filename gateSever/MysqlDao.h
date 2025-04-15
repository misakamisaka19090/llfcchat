#pragma once
#include "const.h"
#include <thread>

//封装 MySQL 数据库连接对象和其操作时间戳
class SqlConnection {
public:
	//_con：智能指针管理的 MySQL 连接对象
	//_last_oper_time：记录该连接上次操作的时间戳
	SqlConnection(sql::Connection* con, int64_t lasttime) :_con(con), _last_oper_time(lasttime) {}
	std::unique_ptr<sql::Connection> _con;
	int64_t _last_oper_time;
};


//简单的 MySQL 连接池管理器，自动创建、维护、复用连接。
class MySqlPool {
public:
	//初始化连接池：创建指定数量的连接并压入队列；
	MySqlPool(const std::string& url, const std::string& user, const std::string& pass, const std::string& schema, int poolSize)
		: url_(url), user_(user), pass_(pass), schema_(schema), poolSize_(poolSize), b_stop_(false) {
		try {
			for (int i = 0; i < poolSize_; ++i) {
				//获取 MySQL 的驱动实例
				sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
				//与 MySQL 数据库建立 TCP 连接。
				auto* con = driver->connect(/*"tcp://" + */url_, user_, pass_);
				//设置当前使用的数据库
				con->setSchema(schema_);
				// 获取当前时间戳
				auto currentTime = std::chrono::system_clock::now().time_since_epoch();
				// 将时间戳转换为秒
				long long timestamp = std::chrono::duration_cast<std::chrono::seconds>(currentTime).count();
				pool_.push(std::make_unique<SqlConnection>(con, timestamp));
			}

			//启动后台线程 _check_thread，每60秒调用一次 checkConnection()，直到 b_stop_ == true。
			_check_thread = std::thread([this]() {
				while (!b_stop_) {
					checkConnection();
					std::this_thread::sleep_for(std::chrono::seconds(60));
				}
				});

			_check_thread.detach();
		}
		catch (sql::SQLException& e) {
			// 处理异常
			std::cout << "mysql pool init failed, error is " << e.what() << std::endl;
			//std::cout << "Error code: " << e.getErrorCode() << std::endl;
			//std::cout << "SQLState: " << e.getSQLState() << std::endl;
		}
	}

	//定期检查连接活性，失效连接重新创建
	void checkConnection() {
		//自动加锁，保护 pool_ 队列的线程安全，std::lock_guard 会在作用域结束时自动解锁（RAII原理）
		std::lock_guard<std::mutex> guard(mutex_);
		int poolsize = pool_.size();
		// 获取当前时间戳
		auto currentTime = std::chrono::system_clock::now().time_since_epoch();
		// 将时间戳转换为秒
		long long timestamp = std::chrono::duration_cast<std::chrono::seconds>(currentTime).count();
		for (int i = 0; i < poolsize; i++) {
			auto con = std::move(pool_.front());
			pool_.pop();
			//Defer 析构时调用输入的函数  现自动还回连接池
			Defer defer([this, &con]() {
				pool_.push(std::move(con));
				});

			if (timestamp - con->_last_oper_time < 5) {
				continue;
			}

			try {
				std::unique_ptr<sql::Statement> stmt(con->_con->createStatement());
				stmt->executeQuery("SELECT 1");
				con->_last_oper_time = timestamp;
				//std::cout << "execute timer alive query , cur is " << timestamp << std::endl;
			}
			catch (sql::SQLException& e) {
				std::cout << "Error keeping connection alive: " << e.what() << std::endl;
				// 重新创建连接并替换旧的连接
				sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
				auto* newcon = driver->connect(url_, user_, pass_);
				newcon->setSchema(schema_);
				con->_con.reset(newcon);
				con->_last_oper_time = timestamp;
			}
		}
	}

	//从连接池 pool_ 中安全地拿出一个连接对象（SqlConnection），保证线程安全，并把这个连接的“唯一所有权”返回给调用者。
	std::unique_ptr<SqlConnection> getConnection() {
		// 立即调用 mutex_.lock() 加锁
		// 当 lock 生命周期结束时（出作用域），自动调用 mutex_.unlock() 解锁
		std::unique_lock<std::mutex> lock(mutex_);
		//如果池子为空，返回0，0就等待
		cond_.wait(lock, [this] {
			if (b_stop_) {
				return true;
			}
			return !pool_.empty(); });
		if (b_stop_) {
			return nullptr;
		}
		std::unique_ptr<SqlConnection> con(std::move(pool_.front()));
		pool_.pop();
		return con;
	}

	//将连接返回连接池
	void returnConnection(std::unique_ptr<SqlConnection> con) {
		// mutex_.lock() 加锁
		std::unique_lock<std::mutex> lock(mutex_);
		if (b_stop_) {
			return;
		}
		pool_.push(std::move(con)); // 把连接放回池子
		cond_.notify_one();	// 通知正在等待连接的线程（唤醒一个）
	}

	//关闭连接池，释放所有资源
	void Close() {
		b_stop_ = true;        // 设置停止标志，表示连接池即将关闭
		cond_.notify_all();    // 唤醒所有正在等待的线程
	}

	~MySqlPool() {
		std::unique_lock<std::mutex> lock(mutex_);
		while (!pool_.empty()) {
			pool_.pop();
		}
	}

private:
	std::string url_;
	std::string user_;
	std::string pass_;
	std::string schema_;
	int poolSize_;
	std::queue<std::unique_ptr<SqlConnection>> pool_;
	std::mutex mutex_;
	std::condition_variable cond_;
	std::atomic<bool> b_stop_;
	std::thread _check_thread;
};

struct UserInfo {
	std::string name;
	std::string pwd;
	int uid;
	std::string email;
};



/**
 * @brief MySQL 数据访问对象类，封装对用户数据的数据库操作
 */
class MysqlDao
{
public:
	/**
	 * @brief 构造函数，初始化数据库连接池
	 */
	MysqlDao();

	/**
	 * @brief 析构函数，关闭数据库连接池
	 */
	~MysqlDao();

	/**
	 * @brief 注册用户（使用存储过程实现）
	 * @param name 用户名
	 * @param email 用户邮箱
	 * @param pwd 用户密码
	 * @return 成功返回用户ID，失败返回-1
	 */
	int RegUser(const std::string& name, const std::string& email, const std::string& pwd);

	/**
	 * @brief 使用事务方式注册用户（包含完整性检查）
	 * @param name 用户名
	 * @param email 用户邮箱
	 * @param pwd 用户密码
	 * @param icon 用户头像
	 * @return 成功返回用户ID，失败返回-1
	 */
	int RegUserTransaction(const std::string& name, const std::string& email, const std::string& pwd, const std::string& icon);

	/**
	 * @brief 检查用户名和邮箱是否匹配
	 * @param name 用户名
	 * @param email 用户邮箱
	 * @return 匹配返回true，否则返回false
	 */
	bool CheckEmail(const std::string& name, const std::string& email);

	/**
	 * @brief 更新用户密码
	 * @param name 用户名
	 * @param newpwd 新密码
	 * @return 更新成功返回true，失败返回false
	 */
	bool UpdatePwd(const std::string& name, const std::string& newpwd);

	/**
	 * @brief 验证用户名和密码
	 * @param name 用户名
	 * @param pwd 密码
	 * @param userInfo 输出参数，验证成功时填充用户信息
	 * @return 验证成功返回true，失败返回false
	 */
	bool CheckPwd(const std::string& name, const std::string& pwd, UserInfo& userInfo);

	/**
	 * @brief 测试调用存储过程
	 * @param email 输入邮箱
	 * @param uid 输出参数，存储过程返回的用户ID
	 * @param name 输出参数，存储过程返回的用户名
	 * @return 调用成功返回true，失败返回false
	 */
	bool TestProcedure(const std::string& email, int& uid, std::string& name);

private:
	std::unique_ptr<MySqlPool> pool_;  ///< MySQL连接池智能指针
};


