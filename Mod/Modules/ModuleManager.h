#pragma once
#include <vector>
#include "Module.h"

class ModuleManager {
private:
	std::vector<Module*> moduleList = std::vector<Module*>();
	bool isInit = false;
public:
	auto Init()->void;
	auto Disable()->void;
	auto IsInitialized()->bool;

	template <typename TRet>
	auto GetModule()->TRet;

public:
	auto onKeyUpdate(int, bool)->void;
	auto onTick(class GameMode*)->void;
	auto onAttack(class Actor*)->bool;							// 返回值可以拦截该事件
	auto onKnockback(class LocalPlayer*, struct vec3_t*)->bool;  // 返回值可以拦截该事件
};

template <typename TRet>
auto ModuleManager::GetModule()->TRet {
	if (!IsInitialized())
		return nullptr;
	for (auto pMod : moduleList) {
		if (auto pRet = dynamic_cast<typename std::remove_pointer<TRet>::type*>(pMod)) {
		//if (auto pRet = dynamic_cast<TRet>(pMod)) {
			return pRet;
		}
	}
	return nullptr;
}