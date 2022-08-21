#pragma once
#include <vector>
#include "Module.h"

class ModuleManager {
private:
	static std::vector<Module*> moduleList;
	static bool isInit;
public:
	static auto Init()->void;
	static auto Disable()->void;
	static auto IsInitialized()->bool;

	template <typename TRet>
	static auto GetModule()->TRet*;

public:
	static auto onKeyUpdate(int, bool)->void;
	static auto onTick(class GameMode*)->void;
	static auto onAttack(class Actor*)->bool;							// 返回值可以拦截该事件
	static auto onKnockback(class LocalPlayer*, struct vec3_t*)->bool; // 返回值可以拦截该事件
	static auto onStartDestroyBlock(class GameMode* gm, struct vec3_ti* Bpos, uint8_t* Face)->void;
};