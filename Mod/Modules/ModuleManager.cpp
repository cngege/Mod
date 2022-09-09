#include "ModuleManager.h"
//模块
#include "Modules/HitBox.h"
#include "Modules/InstantDestroy.h"
#include "Modules/NoKnockback.h"
#include "Modules/Traverse.h"
#include "Modules/ShowCoordinates.h"
#include "Modules/TPPoint.h"
#include "Modules/Render.h"
#include "Modules/NoAttackFriend.h"
#include "Modules/ArmsLength.h"
#include "Modules/HiveTreasureHitBox.h"

//bool ModuleManager::isInit = false;
//std::vector<Module*> ModuleManager::moduleList = std::vector<Module*>();

//载入所有的Module到列表中
//每新建一个Module都在这里添加一次 new 
auto ModuleManager::Init()->void {
	if (isInit) return;

	moduleList.push_back((Module*)(new HitBox()));
	moduleList.push_back((Module*)(new HiveTreasureHitBox()));
	moduleList.push_back((Module*)(new InstantDestroy()));
	moduleList.push_back((Module*)(new NoKnockback()));
	moduleList.push_back((Module*)(new Traverse()));
	moduleList.push_back((Module*)(new ShowCoordinates()));
	moduleList.push_back((Module*)(new TPPoint()));
	moduleList.push_back((Module*)(new Render()));
	moduleList.push_back((Module*)(new NoAttackFriend()));
	moduleList.push_back((Module*)(new ArmsLength()));

	isInit = true;
}


//关闭所有的 Module 并执行析构函数
auto ModuleManager::Disable()->void {
	if (!IsInitialized())
		return;
	for (auto pMod : moduleList) {
		if(pMod->isEnabled())
			pMod->setEnabled(false);
	}
	for (auto pMod : moduleList) {
		delete[] pMod;
	}
	isInit = false;
	moduleList.clear();
}

auto ModuleManager::IsInitialized()->bool {
	return isInit;
}

auto ModuleManager::GetAllModule()->std::vector<Module*> {
	return moduleList;
}

auto ModuleManager::Moduleforeach(std::function<void(Module*)> callback)->void {
	if (!IsInitialized())
		return;
	for (auto pMod : moduleList) {
		callback(pMod);
	}
}

auto ModuleManager::onKeyUpdate(int key, bool isenable)->void {
	if (!IsInitialized())
		return;
	for (auto pMod : moduleList) {
		auto bindkey = pMod->getKeybind();
		if (bindkey != 0 && bindkey == key && pMod->checkcontrolkeys()) {
			switch (pMod->GetKeyMode())
			{
			case Hold:
				pMod->setEnabled(isenable);
				break;
			case Trigger:
				if (isenable) {
					pMod->onTrigger();
				}
				break;
			case Switch:
				if (isenable) {
					pMod->toggle();
				}
				break;
			}
		}
		pMod->onKeyUpdate(key, isenable);
	}
}

auto ModuleManager::onTick(GameMode* gm)->void {
	if (!IsInitialized())
		return;
	for (auto pMod : moduleList) {
		pMod->onTick(gm);
	}
}

auto ModuleManager::onAttack(Actor* actor)->bool {
	bool RunOriginalFun = true;
	if (!IsInitialized())
		return RunOriginalFun;
	for (auto pMod : moduleList) {
		if (!pMod->onAttack(actor))
			RunOriginalFun = false;
	}
	return RunOriginalFun;
}

auto ModuleManager::onKnockback(LocalPlayer* lp, vec3_t* v3)->bool {
	bool RunOriginalFun = true;
	if (!IsInitialized())
		return RunOriginalFun;
	for (auto pMod : moduleList) {
		if (!pMod->onKnockback(lp,v3))
			RunOriginalFun = false;
	}
	return RunOriginalFun;
}

auto ModuleManager::onActorTick(Actor* actor)->void {
	if (!IsInitialized())
		return;
	for (auto pMod : moduleList) {
		pMod->onActorTick(actor);
	}
}

auto ModuleManager::onLocalPlayerTick(LocalPlayer* lp)->void {
	if (!IsInitialized())
		return;
	for (auto pMod : moduleList) {
		pMod->onLocalPlayerTick(lp);
	}
}


auto ModuleManager::onRenderDetour(MinecraftUIRenderContext* ctx)->void {
	if (!IsInitialized())
		return;
	for (auto pMod : moduleList) {
		pMod->onRenderDetour(ctx);
	}
}