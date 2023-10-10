#include "config.h"
#include <io.h>
#include "Utils.h"
//#include "Logger.h"
//#include <fstream>
//#include <iostream>
//#include <vector>

std::string config::currentSaveConfigFile = "Default";					// 指示当前使用的配置文件的默认值 也就是默认使用 Default.json 这个配置文件

std::string config::getConfigFilePath() {
	static std::string configFilePath = Utils::WStringToString(Utils::GetRoamingFolderPath()) + "\\Mod\\Config\\";
	return configFilePath;
}


std::vector<std::string> config::findAllConfigFile() {
	std::vector<std::string> files;
	intptr_t hFile = 0;
	struct _finddata_t fileinfo;
	if ((hFile = _findfirst(getConfigFilePath().append("*.json").c_str(),&fileinfo)) != -1) {
		do {
			if (!(fileinfo.attrib & _A_SUBDIR)) {
				if (strcmp(fileinfo.name, "config.json") != 0) {				// config.json 作为固定配置,和其他保存模块信息等配置不一样
					auto fname = std::string(fileinfo.name);
					auto find = fname.find(".json");
					files.push_back(fname.substr(0, find));
				}
			}
			else {
				//递归查询内部文件夹 这里不需要
			}
		} while (_findnext(hFile, &fileinfo) == 0);
	}
	_findclose(hFile);
	return files;
}



json config::loadConfigonRootFromFile(const std::string& name) {
	json ret = {};
	std::ifstream f((getConfigFilePath() + (Utils::utf8_check_is_valid(name) ? Utils::UTF8toANSI(name.c_str()) : name) + ".json").c_str());
	if (f.good())								//表示配置文件存在
	{
		f >> ret;
		f.close();
	}
	return ret;
}

json config::loadConfigonRootFromDefaultFile() {
	return loadConfigonRootFromFile("Default");
}

json config::loadConfigFromFile(const std::string& name, std::string key) {
	auto root = loadConfigonRootFromFile(name);
	if (root.contains(key)) {
		return root[key];
	}
	else {
		return {};
	}
}

json config::loadConfigFromDefaultFile(std::string key) {
	return loadConfigFromFile("Default", key);
}

// 写入配置
bool config::writeConfigonRootToFile(const std::string& name, json data) {
	std::ofstream c((getConfigFilePath() + (Utils::utf8_check_is_valid(name) ? Utils::UTF8toANSI(name.c_str()) : name) + ".json").c_str());
	c << data.dump(2);
	c.close();
	return true;
}

bool config::writeConfigonRootFromDefaultFile(json data) {
	return writeConfigonRootToFile("Default", data);
}

