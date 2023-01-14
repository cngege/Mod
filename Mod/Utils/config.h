#pragma once
#include <string>
#include "Json.hpp"

using namespace nlohmann;

class config {
public:
	static std::string currentSaveConfigFile;
public:
	//static bool setconfigFile(const std::string& name);								// 无需后缀 统一后缀.json
	static std::string getConfigFilePath();												// 获取配置文件的目录位置 \\结尾
	static std::vector<std::string> findAllConfigFile();								// 从配置文件目录中 读取所有后缀为json的文件
	static json loadConfigonRootFromFile(const std::string& name);						// 读取某个配置文件中的json,返回整个json内容
	static json loadConfigonRootFromDefaultFile();										// 读取默认配置文件中的json,返回整个json内容
	static json loadConfigFromFile(const std::string& name, std::string key);			// 读取某个配置文件中的json,返回指定key的json内容
	static json loadConfigFromDefaultFile(std::string key);								// 读取默认配置文件中的json,返回指定key的json内容(默认配置文件:Default.json)

	static bool writeConfigonRootToFile(const std::string& name, json data);			// 将json写入到整个配置中
	static bool writeConfigonRootFromDefaultFile(json data);							// 将json完全写到默认配置文件中

	template <class T>
	static T readDataFromJson(json data, std::string key, T defdata);
};

template <class T>
T config::readDataFromJson(json data, std::string key, T defdata) {
	if (data.contains(key)) {
		return data[key];
	}
	else {
		return defdata;
	}
}