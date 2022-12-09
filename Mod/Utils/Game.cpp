#include "Game.h"
#include "../Utils/Logger.h"
#include "../Utils/Utils.h"
#include "Player.h"
#include "Actor.h"
#include "AttributeInstance.h"
#include "GameMode.h"
#include "LocalPlayer.h"
#include "Item.h"
#include "ItemStack.h"

#include "../Modules/ModuleManager.h"


bool Game::ModState = false;
ModuleManager* Game::modmag = nullptr;
LocalPlayer* Game::localplayer = nullptr;
BitmapFont* Game::mcfont = nullptr;
ClientInstance* Game::Cinstance = nullptr;

uintptr_t Game::KeyMap = 0x00;
bool Game::MouseKeyDown[10] = { false };

auto Game::init() -> void
{
	logF("[Game::init] is start runner");

	Game::ModState = true;
	Game::modmag = new ModuleManager;
	Game::modmag->Init();
	logF("[ModuleManager::Disable] now init");


	//获取生物位置指针的偏移
	{
		Mob::setSprintingFunAddr = FindSignature("48 89 74 24 ? 57 48 83 EC ? 48 8B ? 0F B6 F2 BA");
		if (Mob::setSprintingFunAddr == 0x00) {
			logF("[Game::init] [Warn]Find Mob::setSprintingFunAddr FunAddr is no working!!!,Mob::setSprintingFunAddr=0");
			return;
		}
	}

	//Actor::isRemoved() 获取构成该函数的 this到结果的关键偏移
	{
		auto offset = FindSignature("53 48 83 EC ? 80 BA ? ? ? ? 0 48 8B D9 75 ? 48 8B 81 ? ? ? ? 48 85 C0");
		if (offset == 0x00) {
			logF("[Game::init] [Warn]Find Actor::isRemoved() offset is no working!!!");
			return;
		}
		else {
			Actor::IsRemovedOffset = *reinterpret_cast<int*>(offset + 7);
		}
	}

	//获取玩家视角的指针的偏移地址
	{
		auto PlayerView_sigOffset = FindSignature("48 8B 88 ? ? ? ? 48 85 C9 0F 84 ? ? ? ? F3 0F 10 80");
		if (PlayerView_sigOffset == 0x00) {
			logF("[Game::init] [Warn]Find Player PlayerView_sigOffset Offset is no working!!!,PlayerView_sigOffset=0");
			return;
		}
		Player::RotPtrOffset = *reinterpret_cast<int*>(PlayerView_sigOffset + 3);
		//Player::Rot1 = offset;
		//Player::Rot2 = offset + 8;
	}

	//获取 获取玩家血量函数的相关偏移
	{
		auto getHealthFun_sigOffset = FindSignature("48 8D 15 ? ? ? ? 48 8B 80 ? ? ? ? FF 15 ? ? ? ? F3 0F 10 88 ? ? ? ? F3 0F 2C C1 66 0F 6E");
		if (getHealthFun_sigOffset == 0x00) {
			logF("[Game::init] [Warn]Find Player getHealthFun_sigOffset Offset is no working!!!,getHealthFun_sigOffset=0");
			return;
		}
		auto offset = *reinterpret_cast<int*>(getHealthFun_sigOffset + 3);
		auto HEALTHVal = *reinterpret_cast<__int64*>(getHealthFun_sigOffset + 7 + offset);

		auto vtoffset = *reinterpret_cast<int*>(getHealthFun_sigOffset + 10);
		auto gvoffset = *reinterpret_cast<int*>(getHealthFun_sigOffset + 24);
		Actor::GetAttributeInstance_HealthFunVT = vtoffset;// Actor::GetAttributeInstance_HealthFunVT 疑似不需要
		AttributeInstance::getCurrentValueoffset = gvoffset;


		if (Attribute::HEALTH != HEALTHVal) {
			logF("[Game::init] [Error]Attribute::HEALTH is Error,should is :%I64d", HEALTHVal);
		}
		if (vtoffset == 0x00) {
			logF("[Game::init] [Error]vtoffset == 0x00 ");
		}
		if (gvoffset == 0x00) {
			logF("[Game::init] [Error]gvoffset == 0x00 ");
		}
	}

	//KeyMap
	{
		auto sigOffset = FindSignature("4C 8D 05 ? ? ? ? 89 54 24 ? 88 4C 24");
		if (sigOffset == 0x0) {
			logF("[Game::init] [Warn]Find KeyMap Offset is no working!!!,KeyMap=0");
			return;
		}
		int offset = *reinterpret_cast<int*>((sigOffset + 3));      // Get Offset from code
		KeyMap = sigOffset + offset + /*length of instruction*/ 7;  // Offset is relative
		logF("KeyMap: %llX", KeyMap);
	}

	//获取本地玩家OnGround函数的偏移
	{
		auto sigOffset = FindSignature("80 B8 ? ? ? ? 00 75 ? 48 8B C8 E8 ? ? ? ? 84");
		if (sigOffset == 0x0) {
			logF("[Game::init] [Warn]Find OnGround Offset is no working!!!,OnGround=0");
			return;
		}
		LocalPlayer::onGroundoffset = *reinterpret_cast<int*>(sigOffset + 2);      // Get Offset from code
	}

	//获取 Item::getIdEx 所用到的偏移地址 getIdOffset;  特征码来自
	{
		auto getIdOffset = FindSignature("0F B7 B8 ? ? ? ? 89 B5 ? ? ? ? 4C 89");
		if (getIdOffset == 0x0) {
			logF("[Game::init] [Warn]Find Item::getIdEx Offset is no working!!!,getIdOffset=0");
			return;
		}
		Item::getIdOffset = *reinterpret_cast<int*>(getIdOffset + 3);      // Get Offset from code
	}

	//获取 ItemStack::use 这个Call
	{
		auto ItemStack_use = FindSignature("48 89 74 24 ? 57 48 83 EC ? 48 8B 41 08 48 8B F2 48 8B F9");								//直接特征码搜索这个Call
		if (ItemStack_use == 0x00) {
			logF("[Game::init] [!]Find ItemStack_use Call 直接特征码搜索函数失败,将使用第二种方式");
			auto ItemStack_use2 = FindSignature("E8 ? ? ? ? 48 8B D8 48 8B D0 49 8B CE");
			if (ItemStack_use2 == 0x00) {
				logF("[Game::init] [Error]Find ItemStack_use 两种路径查找均失败");
				return;
			}
			else {
				ItemStack::useCall = Utils::FuncFromSigOffset<uintptr_t*>(ItemStack_use2, 1);
			}
		}
		else {
			ItemStack::useCall = (uintptr_t*)ItemStack_use;
		}
	}
}


auto Game::exit() -> void
{
	Game::modmag->Disable();
	delete[] Game::modmag;
	logF("[ModuleManager::Disable] now Disale");

	Game::ModState = false;
}

auto Game::GetModuleManager()->ModuleManager* {
	return Game::modmag;
}


auto Game::IsKeyDown(int key)->bool {
	// All keys are mapped as bools, though aligned as ints (4 byte)
	// key0 00 00 00 key1 00 00 00 key2 00 00 00 ...
	return *reinterpret_cast<bool*>(KeyMap + ((uintptr_t)key * 0x4));
}

/// <summary>
/// 判断一个按键是否按下
/// </summary>
/// <param name="key"></param>
/// <returns>如果判断时已经按下，则等到松开时再返回true</returns>
auto Game::IsKeyPressed(int key)->bool {
	if (IsKeyDown(key)) {
		while (IsKeyDown(key))
			Sleep(1);
		return true;
	}
	return false;
}

auto Game::IsMouseDown(int key) -> bool
{
	return MouseKeyDown[key];
}
