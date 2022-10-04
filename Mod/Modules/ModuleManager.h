#pragma once
#include <vector>
#include "Module.h"
#include <functional>

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

	auto GetAllModule()->std::vector<Module*>;
	auto Moduleforeach(std::function<void(Module*)>)->void;

public:
	auto onKeyUpdate(int, bool)->void;
	auto onMouseUpdate(char mousebutton, char isdown, __int16 mouseX, __int16 mouseY, __int16 relativeMovementX, __int16 relativeMovementY)->void;
	auto onTick(class GameMode*)->void;
	auto onAttack(class Actor*)->bool;							// 返回值可以拦截该事件
	bool useItem(class GameMode*, class ItemStack*);
	bool useItemOn(class GameMode*, class ItemStack*, vec3_ti*, uint8_t*, struct vec3_t*, class Block*);
	auto onKnockback(class LocalPlayer*, struct vec3_t*)->bool;  // 返回值可以拦截该事件
	auto onActorTick(class Actor*)->void;
	auto onLocalPlayerTick(class LocalPlayer*)->void;
	auto onRenderDetour(class MinecraftUIRenderContext*)->void;
	auto onImGUIRender()->void;
	auto onSendMessage(class TextHolder*)->bool;
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