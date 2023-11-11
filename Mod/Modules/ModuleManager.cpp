#include "ModuleManager.h"
#include "../Utils/config.h"
//模块
#include "Modules/HitBox.h"
#include "Modules/AirJump.h"
#include "Modules/GameTimeLock.h"
#include "Modules/InstantDestroy.h"
#include "Modules/NoKnockback.h"
#include "Modules/Traverse.h"
#include "Modules/ShowCoordinates.h"
#include "Modules/TPPoint.h"
#include "Modules/Render.h"
#include "Modules/NoAttackFriend.h"
#include "Modules/ArmsLength.h"
//#include "Modules/AutoWalking.h"
#include "Modules/HiveTreasurePos.h"
#include "Modules/RenderHealth.h"
//#include "Modules/AutoSprinting.h"
#include "Modules/BioRadar.h"
#include "Modules/FastViewPerspective.h"
#include "Modules/LockControlInput.h"
#include "Modules/ChangeGameMode.h"
#include "Modules/FreeCamera.h"
#include "Modules/HundredTimesMoreDrops.h"

#ifdef _DEBUG
#include "Modules/Debug.h"
#endif

//bool ModuleManager::isInit = false;
//std::vector<Module*> ModuleManager::moduleList = std::vector<Module*>();

//载入所有的Module到列表中
//每新建一个Module都在这里添加一次 new 
auto ModuleManager::Init()->void {
	if (isInit) return;

	// 读取所有模块,和配置文件,给所有模块加载配置文件 且这里放在所有模块的构造函数前面, 这样在模块的构造函数中就可以直接使用 config::currentSaveConfigFile
	json sysdata = config::loadConfigonRootFromFile("config");
	if (sysdata.contains("CurrentConfigFile")) {
		config::currentSaveConfigFile = sysdata["CurrentConfigFile"];
	}

	moduleList.push_back((Module*)(new HitBox()));							/*Caps*/
	moduleList.push_back((Module*)(new AirJump()));
	moduleList.push_back((Module*)(new GameTimeLock()));
	moduleList.push_back((Module*)(new InstantDestroy()));					/*Ctrl+Shift*/
	moduleList.push_back((Module*)(new Traverse()));						/*F2*/
	moduleList.push_back((Module*)(new ShowCoordinates()));
	moduleList.push_back((Module*)(new TPPoint()));							/*Ctrl/Shift+F3*/
	moduleList.push_back((Module*)(new Render()));							/*INS*/
	moduleList.push_back((Module*)(new NoKnockback()));						/*Ctrl+F4*/
	moduleList.push_back((Module*)(new NoAttackFriend()));					/*SHIFT+F4*/
	moduleList.push_back((Module*)(new BioRadar()));						/*SHIFT+F6*/
	moduleList.push_back((Module*)(new HiveTreasurePos()));					/*SHIFT+F7*/
	moduleList.push_back((Module*)(new FastViewPerspective()));				/*(R) F9*/
	moduleList.push_back((Module*)(new ArmsLength()));						/*SHIFT+F10*/
	moduleList.push_back((Module*)(new RenderHealth()));					/*Ctrl+F10*/
																				/*F11 = 全屏*/
	moduleList.push_back((Module*)(new LockControlInput()));				/*Ctrl+F12*/
	moduleList.push_back((Module*)(new FreeCamera()));
	moduleList.push_back((Module*)(new ChangeGameMode()));
	moduleList.push_back((Module*)(new HundredTimesMoreDrops()));
#ifdef _DEBUG
	moduleList.push_back((Module*)(new Debug()));
#endif

	isInit = true;

#ifndef _REPAIR
	// 读取 指定的配置文件
	json configdata = config::loadConfigonRootFromFile(config::currentSaveConfigFile);
	for (auto& mod : moduleList) {										// 这里有&则代表引用,所有的修改都将作用在原来的数组成员上
		mod->onloadConfigFile(configdata[mod->getModuleName()]);
	}
#endif // !_REPAIR
}


//关闭所有的 Module
auto ModuleManager::Disable()->void {
	if (!IsInitialized())
		return;
	for (auto pMod : moduleList) {
		if(pMod->isEnabled())
			pMod->setEnabled(false);
	}
	for (auto pMod : moduleList) {
		delete pMod;
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

auto ModuleManager::onMouseUpdate(char mousebutton, char isdown, __int16 mouseX, __int16 mouseY, __int16 relativeMovementX, __int16 relativeMovementY)->void {
	if (!IsInitialized())
		return;
	for (auto pMod : moduleList) {
		pMod->onMouseUpdate(mousebutton, isdown, mouseX, mouseY, relativeMovementX, relativeMovementY);
	}
}

auto ModuleManager::onTick(GameMode* gm)->void {
	if (!IsInitialized())
		return;
	for (auto pMod : moduleList) {
		pMod->onTick(gm);
	}
}

auto ModuleManager::onAttackBefore(class GameMode* gm ,Actor* actor)->bool {
	bool RunOriginalFun = true;
	if (!IsInitialized())
		return RunOriginalFun;
	for (auto pMod : moduleList) {
		if (!pMod->onAttackBefore(gm, actor))
			RunOriginalFun = false;
	}
	return RunOriginalFun;
}

auto ModuleManager::onAttackAfter(class GameMode* gm ,Actor* actor)->void {
	if (!IsInitialized())
		return;
	for (auto pMod : moduleList) {
		pMod->onAttackAfter(gm, actor);
	}
}

auto ModuleManager::useItem(GameMode* gm, class ItemStack* item)->bool {
	bool RunOriginalFun = true;
	if (!IsInitialized())
		return RunOriginalFun;
	for (auto pMod : moduleList) {
		if (!pMod->useItem(gm,item))
			RunOriginalFun = false;
	}
	return RunOriginalFun;
}

auto ModuleManager::useItemOn(GameMode* gm, class ItemStack* item, class ItemInstance* itemins, vec3_ti* bpos, uint8_t* face, vec3_t* f, class Block* block)->bool {
	bool RunOriginalFun = true;
	if (!IsInitialized())
		return RunOriginalFun;
	for (auto pMod : moduleList) {
		if (!pMod->useItemOn(gm, item,itemins,bpos,face, f, block))
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

auto ModuleManager::onActorSightTick(Actor* actor) -> void
{
	if (!IsInitialized())
		return;
	for (auto pMod : moduleList) {
		pMod->onActorSightTick(actor);
	}
}

auto ModuleManager::onPlayerTick(Player* player) -> void
{
	if (!IsInitialized())
		return;
	for (auto pMod : moduleList) {
		pMod->onPlayerTick(player);
	}
}

auto ModuleManager::onPlayerSightTick(Player* player) -> void
{
	if (!IsInitialized())
		return;
	for (auto pMod : moduleList) {
		pMod->onPlayerSightTick(player);
	}
}

auto ModuleManager::onServerPlayerTick(ServerPlayer* serverplayer) -> void
{
	if (!IsInitialized())
		return;
	for (auto pMod : moduleList) {
		pMod->onServerPlayerTick(serverplayer);
	}
}

auto ModuleManager::onLocalPlayerTick(LocalPlayer* lp)->void {
	if (!IsInitialized())
		return;
	for (auto pMod : moduleList) {
		pMod->onLocalPlayerTick(lp);
	}
}

auto ModuleManager::onRemotePlayerTick(RemotePlayer* remoteplayer) -> void
{
	if (!IsInitialized())
		return;
	for (auto pMod : moduleList) {
		pMod->onRemotePlayerTick(remoteplayer);
	}
}

auto ModuleManager::onLevelTick(Level* level)->void {
	if (!IsInitialized())
		return;
	for (auto pMod : moduleList) {
		pMod->onLevelTick(level);
	}
}

auto ModuleManager::onstartLeaveGame(Level* level) -> void
{
	if (!IsInitialized())
		return;
	for (auto pMod : moduleList) {
		pMod->onstartLeaveGame(level);
	}
}

auto ModuleManager::onRenderDetour(MinecraftUIRenderContext* ctx)->void {
	if (!IsInitialized())
		return;
	for (auto pMod : moduleList) {
		pMod->onRenderDetour(ctx);
	}
}

auto ModuleManager::onImGUIRender()->void {
	if (!IsInitialized())
		return;
	for (auto pMod : moduleList) {
		pMod->onImGUIRender();
	}
}

auto ModuleManager::onSendMessage(std::mcstring* TH)->bool {
	bool RunOriginalFun = true;
	if (!IsInitialized())
		return RunOriginalFun;
	for (auto pMod : moduleList) {
		if (!pMod->onSendMessage(TH))
			RunOriginalFun = false;
	}
	return RunOriginalFun;
}

auto ModuleManager::onSendPacketToServer(LoopbackPacketSender* sender, Packet* pack) -> bool
{
	bool RunOriginalFun = true;
	if (!IsInitialized())
		return RunOriginalFun;
	for (auto pMod : moduleList) {
		if (!pMod->onSendPackToServer(sender, pack))
			RunOriginalFun = false;
	}
	return RunOriginalFun;
}

auto ModuleManager::onloadConfigFile(json& data)->void {
	if (!IsInitialized())
		return;
	for (auto pMod : moduleList) {
		pMod->onloadConfigFile(data);
	}
}

auto ModuleManager::onsaveConfigFile(json& data)->void {
	if (!IsInitialized())
		return;
	for (auto pMod : moduleList) {
		pMod->onsaveConfigFile(data);
	}
}