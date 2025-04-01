#ifndef SINGLETON_H
#define SINGLETON_H

#include <global.h>

/******************************************************************************
 *
 * @file        singleton.h
 * @brief       线程安全的单例模式模板类
 *              该类是一个通用的单例模式实现，确保在多线程环境下，只会创建一个实例。
 *              通过删除拷贝构造函数和赋值运算符，防止用户错误地复制单例对象，确保始终只有一个实例存在。
 *              采用C++11中的 `std::call_once` 和 `std::once_flag` 来保证线程安全
 *
 * @author      misaka
 * @date        2025/03/28
 * @history     无修改记录
 *****************************************************************************/

template <typename T>
class Singleton {
protected:
    /**
     * @brief 默认构造函数，构造函数为protected，禁止外部创建实例。
     */
    Singleton() = default;

    /**
     * @brief 禁止拷贝构造函数，避免外部拷贝。
     *
     * @param st 复制的源对象
     */
    Singleton(const Singleton<T>& st) = delete;

    /**
     * @brief 禁止赋值运算符，避免外部赋值。
     *
     * @param st 赋值的源对象
     * @return Singleton& 返回引用本身
     */
    Singleton& operator=(const Singleton<T>& st) = delete;

    /**
     * @brief 静态智能指针，保存单例实例。
     * 通过 `std::shared_ptr` 来管理单例的生命周期，确保资源自动释放。
     */
    static std::shared_ptr<T> _instance;

public:
    /**
     * @brief 获取单例实例。
     * 使用局部静态变量和 `std::call_once` 保证单例实例只会在第一次调用时创建，后续调用将返回同一实例。
     * 该实现是线程安全的。
     *
     * @return std::shared_ptr<T> 单例对象的智能指针
     */
    static std::shared_ptr<T> GetInstance() {
        // 定义局部的静态变量，多次调用时只初始化一次
        static std::once_flag s_flag;
        std::call_once(s_flag, [&]() {
            // T类型的构造函数是protected或者private的，无法通过make_shared直接创建对象。
            _instance = std::shared_ptr<T>(new T);
        });
        return _instance;
    }

    /**
     * @brief 打印单例对象的内存地址。
     * 通过输出对象的内存地址，便于确认单例模式的唯一性。
     */
    void PrintAddress() {
        std::cout << _instance.get() << Qt::endl;
    }

    /**
     * @brief 析构函数。
     * 单例实例被销毁时，输出析构信息。
     */
    ~Singleton() {
        std::cout << "单例实例被销毁" << std::endl;
    }
};

// 定义模板类的静态成员变量
template <typename T>
std::shared_ptr<T> Singleton<T>::_instance = nullptr;

#endif // SINGLETON_H

