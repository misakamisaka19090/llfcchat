#pragma once
#include <fstream>  
#include <boost/property_tree/ptree.hpp>  
#include <boost/property_tree/ini_parser.hpp>  
#include <boost/filesystem.hpp>    
#include <map>
#include <iostream>

/**
 * @brief 用来读取和管理某个配置 Section 的所有键值对 key和value
 */
struct SectionInfo {
	SectionInfo() {}
	~SectionInfo() { _section_datas.clear(); }

	SectionInfo(const SectionInfo& src) {
		_section_datas = src._section_datas;
	}

	SectionInfo& operator=(const SectionInfo& src) {
		if (&src == this)
			return *this;
		_section_datas = src._section_datas;
		return *this;
	}

	std::map<std::string, std::string> _section_datas;
	/**
	 * @brief 通过键获取对应的值
	 * @param key 配置项的名称
	 * @return 如果找到，返回对应的值；否则返回空字符串。
	 */
	std::string  operator[](const std::string& key) {
		if (_section_datas.find(key) == _section_datas.end()) {
			return "";
		}
		// 这里可以添加一些边界检查  
		return _section_datas[key];
	}
};


/**
 * @brief 配置管理器类
 *
 * 该类用于读取和管理配置文件中的内容，将所有 [section] 中的 key=value 配置项存储在 _config_map 中，
 * 以供后续使用。采用单例模式，确保在程序中只有一个配置管理器实例。
 */
class ConfigMgr {
public:
    ~ConfigMgr();  // 析构函数

    /**
     * @brief 按 section 名访问配置数据
     * @param section 配置文件中的 section 名称
     * @return SectionInfo 返回该 section 中的所有键值对
     */
    SectionInfo operator[](const std::string& section);

    /**
     * @brief 获取配置管理器的唯一实例
     * @return ConfigMgr& 返回 ConfigMgr 类的唯一实例
     */
    static ConfigMgr& Inst();

    /**
     * @brief 拷贝赋值操作符
     * @param src 需要复制的 ConfigMgr 对象
     * @return ConfigMgr& 返回当前对象的引用
     */
    ConfigMgr& operator=(const ConfigMgr& src);

    /**
     * @brief 拷贝构造函数
     * @param src 需要复制的 ConfigMgr 对象
     */
    ConfigMgr(const ConfigMgr& src);

private:
    /**
     * @brief 构造函数
     * 该构造函数被私有化，确保只能通过 Inst() 获取实例。
     */
    ConfigMgr();

    // 存储所有 section 和对应的键值对的 map
    std::map<std::string, SectionInfo> _config_map;
};